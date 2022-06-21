/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 RDK Management
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

#include "MessageControl.h"
#include "MessageOutput.h"

namespace WPEFramework {

    namespace Plugin {

    namespace {

        static Metadata<MessageControl> metadata(
            // Version
            1, 0, 0,
            // Preconditions
            {},
            // Terminations
            {},
            // Controls
            {}
        );
    }
    
    MessageControl::Config::NetworkNode::NetworkNode()
        : Core::JSON::Container()
        , Port(2200)
        , Binding("0.0.0.0")
    {
        Add(_T("port"), &Port);
        Add(_T("binding"), &Binding);
    }

    MessageControl::Config::NetworkNode::NetworkNode(const NetworkNode& copy)
        : Core::JSON::Container()
        , Port(copy.Port)
        , Binding(copy.Binding)
    {
        Add(_T("port"), &Port);
        Add(_T("binding"), &Binding);
    }

    MessageControl::MessageControl()
        : _adminLock()
        , _config()
        , _outputDirector()
        , _webSocketExporter()
        , _control(nullptr)
        , _observer(*this)
        , _connectionId(0)
        , _service(nullptr) {
    }

    const string MessageControl::Initialize(PluginHost::IShell* service)
    {
        string message;

        ASSERT(service != nullptr);

        _config.Clear();
        _config.FromString(service->ConfigLine());

        _service = service;
        _service->AddRef();

        RegisterAll();

        _service->Register(&_observer);

        // Lets see if we can create the Message catcher...
        _control = _service->Root<Exchange::IMessageControl>(_connectionId, RPC::CommunicationTimeOut, _T("MessageControlImplementation"));

        if (_control == nullptr) {
            message = _T("MessageControl plugin could not be instantiated.");
        }
        else if (_control->Configure(&_observer) != Core::ERROR_NONE) {
            message = _T("MessageControl plugin could not be _configured.");
        }
        else {
            if ((service->Background() == false) && (((_config.SysLog.IsSet() == false) && (_config.Console.IsSet() == false)) || (_config.Console.Value() == true))) {
                Announce(Core::Messaging::MetaData::MessageType::TRACING, std::make_shared<Publishers::ConsoleOutput>(_config.Abbreviated.Value()));
            }
            if ((service->Background() == true) && (((_config.SysLog.IsSet() == false) && (_config.Console.IsSet() == false)) || (_config.SysLog.Value() == true))) {
                Announce(Core::Messaging::MetaData::MessageType::TRACING, std::make_shared<Publishers::SyslogOutput>(_config.Abbreviated.Value()));
            }
            if (_config.FileName.Value().empty() == false) {
                _config.FileName = service->VolatilePath() + _config.FileName.Value();

                Announce(Core::Messaging::MetaData::MessageType::TRACING, std::make_shared<Publishers::FileOutput>(_config.Abbreviated.Value(), _config.FileName.Value()));
            }
            if ((_config.Remote.Binding.Value().empty() == false) && (_config.Remote.Port.Value() != 0)) {
                std::shared_ptr<Messaging::IMessageOutput> output = std::make_shared<Publishers::UDPOutput>(Core::NodeId(_config.Remote.NodeId()));
                Announce(Core::Messaging::MetaData::MessageType::TRACING, output);
                Announce(Core::Messaging::MetaData::MessageType::LOGGING, output);
            }

            _webSocketExporter.Initialize(service, _config.MaxExportConnections.Value());
        }

        if(message.length() != 0) {
            Deinitialize(service);
        }

        return message;
    }

    void MessageControl::Deinitialize(PluginHost::IShell* service)
    {
        ASSERT (_service == service);

        UnregisterAll();

        _service->Unregister(&_observer);

        _adminLock.Lock();

        _outputDirector.clear();
        _webSocketExporter.Deinitialize();

        _adminLock.Unlock();

        RPC::IRemoteConnection* connection(_service->RemoteConnection(_connectionId));

        VARIABLE_IS_NOT_USED uint32_t result = _control->Release();
        _control = nullptr;

        // It should have been the last reference we are releasing,
        // so it should endup in a DESTRUCTION_SUCCEEDED, if not we
        // are leaking...
        ASSERT(result == Core::ERROR_DESTRUCTION_SUCCEEDED);

        // The process can disappear in the meantime...
        if (connection != nullptr) {
            // But if it did not dissapear in the meantime, forcefully terminate it. Shoot to kill :-)
            connection->Terminate();
            connection->Release();
        }
        _service->Release();
        _service = nullptr;

        _connectionId = 0;
    }

    string MessageControl::Information() const
    {
        // No additional info to report.
        return (string());
    }

    bool MessageControl::Attach(PluginHost::Channel& channel)
    {
        TRACE(Trace::Information, (Core::Format(_T("Attaching channel ID [%d]"), channel.Id()).c_str()));
        return (_webSocketExporter.Attach(channel.Id()));
    }

    void MessageControl::Detach(PluginHost::Channel& channel)
    {
        TRACE(Trace::Information, (Core::Format(_T("Detaching channel ID [%d]"), channel.Id()).c_str()));
        _webSocketExporter.Detach(channel.Id());
    }

    Core::ProxyType<Core::JSON::IElement> MessageControl::Inbound(const string&)
    {
        return (_webSocketExporter.Command());
    }

    Core::ProxyType<Core::JSON::IElement> MessageControl::Inbound(const uint32_t ID, const Core::ProxyType<Core::JSON::IElement>& element)
    {
        return (Core::ProxyType<Core::JSON::IElement>(_webSocketExporter.Received(ID, element)));
    }

} // namespace Plugin
} // namespace WPEFramework
