/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "Monitor.h"

namespace Thunder {
namespace Plugin {

    namespace {

        static Metadata<Monitor> metadata(
            // Version
            1, 0, 1,
            // Preconditions
            { subsystem::TIME },
            // Terminations
            {},
            // Controls
            {}
        );
    }

    static Core::ProxyPoolType<Web::JSONBodyType<Core::JSON::ArrayType<Monitor::Data>>> jsonBodyDataFactory(2);
    static Core::ProxyPoolType<Web::JSONBodyType<Monitor::Data>> jsonBodyParamFactory(2);
    static Core::ProxyPoolType<Web::JSONBodyType<Monitor::Data::MetaData>> jsonMemoryBodyDataFactory(2);

    /* virtual */ const string Monitor::Initialize(PluginHost::IShell* service)
    {
        ASSERT(service != nullptr);

        _config.FromString(service->ConfigLine());

        _skipURL = static_cast<uint8_t>(service->WebPrefix().length());

        Core::JSON::ArrayType<Config::Entry>::Iterator index(_config.Observables.Elements());

        // Create a list of plugins to monitor..
        _monitor.Open(service, index);

        // During the registartion, all Plugins, currently active are reported to the sink.
        service->Register(&_monitor);

        RegisterAll();

        Exchange::JMemoryMonitor::Register(*this, this);

        // On succes return a name as a Callsign to be used in the URL, after the "service"prefix
        return (_T(""));
    }

    /* virtual */ void Monitor::Deinitialize(PluginHost::IShell* service)
    {
        ASSERT(service != nullptr);

        Exchange::JMemoryMonitor::Unregister(*this);

        UnregisterAll();

        service->Unregister(&_monitor);

        _monitor.Close();
    }

    /* virtual */ string Monitor::Information() const
    {
        // No additional info to report.
        return (nullptr);
    }

    /* virtual */ void Monitor::Inbound(Web::Request& request)
    {
        if ((request.Verb == Web::Request::HTTP_PUT) || (request.Verb == Web::Request::HTTP_POST))
            request.Body(jsonBodyParamFactory.Element());
    }

    // <GET> ../				Get all Memory Measurments
    // <GET> ../<Callsign>		Get the Memory Measurements for Callsign
    // <PUT> ../<Callsign>		Reset the Memory measurements for Callsign
    /* virtual */ Core::ProxyType<Web::Response> Monitor::Process(const Web::Request& request)
    {
        ASSERT(_skipURL <= request.Path.length());

        Core::ProxyType<Web::Response> result(PluginHost::IFactories::Instance().Response());
        Core::TextSegmentIterator index(Core::TextFragment(request.Path, _skipURL, static_cast<uint32_t>(request.Path.length() - _skipURL)), false, '/');

        // If there is an entry, the first one will alwys be a '/', skip this one..
        index.Next();

        // By default, we assume everything works..
        result->ErrorCode = Web::STATUS_OK;
        result->Message = "OK";

        if (request.Verb == Web::Request::HTTP_GET) {
            // Let's list them all....
            if (index.Next() == false) {
                if (_monitor.Length() > 0) {
                    Core::ProxyType<Web::JSONBodyType<Core::JSON::ArrayType<Monitor::Data>>> response(jsonBodyDataFactory.Element());

                    _monitor.Snapshot(*response);

                    result->Body(Core::ProxyType<Web::IBody>(response));
                }
            } else {
                MetaData memoryInfo;
                bool operational = false;

                // Seems we only want 1 name
                if (_monitor.Snapshot(index.Current().Text(), memoryInfo, operational) == true) {
                    Core::ProxyType<Web::JSONBodyType<Monitor::Data::MetaData>> response(jsonMemoryBodyDataFactory.Element());

                    *response = Monitor::Data::MetaData(memoryInfo, operational);

                    result->Body(Core::ProxyType<Web::IBody>(response));
                }
            }

            result->ContentType = Web::MIME_JSON;
        } else if ((request.Verb == Web::Request::HTTP_PUT) && (index.Next() == true)) {
            MetaData memoryInfo;
            bool operational = false;

            // Seems we only want 1 name
            if (_monitor.Reset(index.Current().Text(), memoryInfo, operational) == true) {
                Core::ProxyType<Web::JSONBodyType<Monitor::Data::MetaData>> response(jsonMemoryBodyDataFactory.Element());

                *response = Monitor::Data::MetaData(memoryInfo, operational);

                result->Body(Core::ProxyType<Web::IBody>(response));
            }

            result->ContentType = Web::MIME_JSON;
        } else if ((request.Verb == Web::Request::HTTP_POST) && (request.HasBody())) {
            Core::ProxyType<const Monitor::Data> body(request.Body<const Monitor::Data>());
            string observable = body->Observable.Value();

            uint16_t restartWindow = 0;
            uint8_t restartLimit = 0;

            if (body->Restart.IsSet()) {
                restartWindow = body->Restart.Window;
                restartLimit = body->Restart.Limit;
            }
            TRACE(Trace::Information, (_T("Sets Restart Limits:[LIMIT:%d, WINDOW:%d]"), restartLimit, restartWindow));
            _monitor.Update(observable, restartWindow, restartLimit);
        } else {
            result->ErrorCode = Web::STATUS_BAD_REQUEST;
            result->Message = _T(" could not handle your request.");
        }

        return (result);
    }

    void Monitor::NotifyStatusChanged(const string& callsign, const Exchange::IMemoryMonitor::INotification::action action, const Core::OptionalType<Exchange::IMemoryMonitor::INotification::reason> reason) const
    {
        _adminLock.Lock();

        for (auto observer : _monitorObservers) {
            observer->StatusChanged(callsign, action, reason);
        }

        _adminLock.Unlock();

        Exchange::JMemoryMonitor::Event::StatusChanged(*this, callsign, action, reason);
    }

    Core::hresult Monitor::Register(Exchange::IMemoryMonitor::INotification* const notification)
    {
        Core::hresult result = Core::ERROR_ALREADY_CONNECTED;

        ASSERT(notification != nullptr);

        _adminLock.Lock();

        auto it = std::find(_monitorObservers.begin(), _monitorObservers.end(), notification);
        ASSERT(it == _monitorObservers.end());

        if (it == _monitorObservers.end()) {
            notification->AddRef();

            _monitorObservers.push_back(notification);

            result = Core::ERROR_NONE;
        }
        _adminLock.Unlock();

        return (result);
    }

    Core::hresult Monitor::Unregister(const Exchange::IMemoryMonitor::INotification* const notification)
    {
        Core::hresult result = Core::ERROR_ALREADY_RELEASED;

        ASSERT(notification != nullptr);

        _adminLock.Lock();

        auto it = std::find(_monitorObservers.cbegin(), _monitorObservers.cend(), notification);
        ASSERT(it != _monitorObservers.cend());

        if (it != _monitorObservers.cend()) {
            (*it)->Release();

            _monitorObservers.erase(it);

            result = Core::ERROR_NONE;
        }
        _adminLock.Unlock();

        return (result);
    }

    Core::hresult Monitor::RestartingLimits(const string& callsign, const Exchange::IMemoryMonitor::Restart& restart)
    {
        _monitor.Update(callsign, restart.window, restart.limit);

        return (Core::ERROR_NONE);
    }

    Core::hresult Monitor::RestartingLimits(const string& callsign, Exchange::IMemoryMonitor::Restart& restart) const
    {
        _monitor.Snapshot(callsign, restart);

        return (Core::ERROR_NONE);
    }

    Core::hresult Monitor::Observables(Exchange::IMemoryMonitor::IStringIterator*& observables) const
    {
        std::vector<string> list;

        _monitor.Observables(list);

        using Implementation = RPC::IteratorType<RPC::IStringIterator>;
        observables = Core::ServiceType<Implementation>::Create<RPC::IStringIterator>(list);

        return (Core::ERROR_NONE);
    }

    Core::hresult Monitor::MeasurementData(const string& callsign, Exchange::IMemoryMonitor::Statistics& statistics) const
    {
        _monitor.Snapshot(callsign, statistics);

        return (Core::ERROR_NONE);
    }

    Core::hresult Monitor::ResetStatistics(const string& callsign)
    {
        return ((_monitor.Reset(callsign) == true) ? Core::ERROR_NONE : Core::ERROR_UNKNOWN_KEY);
    }
}
}
