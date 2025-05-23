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
 
#include "Packager.h"

#include <interfaces/json/JPackager.h>

namespace Thunder {
namespace Plugin {
namespace {

    constexpr int kMaxValueLength = 256;

}

    namespace {

        static Metadata<Packager> metadata(
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

    const string Packager::Initialize(PluginHost::IShell* service) {
        ASSERT (_service == nullptr);
        ASSERT (service != nullptr);

        _service = service;
        _service->AddRef();
        _skipURL = static_cast<uint8_t>(service->WebPrefix().length());
        _service->Register(&_notification);

        string result;
        _implementation = _service->Root<Exchange::IPackager>(_connectionId, 2000, _T("PackagerImplementation"));
        if (_implementation == nullptr) {
            result = _T("Couldn't create PACKAGER instance ");

        } else {

            PUSH_WARNING(DISABLE_WARNING_DEPRECATED_USE)

            if (_implementation->Configure(_service) == Core::ERROR_NONE) {

            POP_WARNING()

                Exchange::JPackager::Register(*this, _implementation);
            } else {
                result = _T("Couldn't initialize PACKAGER instance");
            }
        }

        return (result);
    }

    void Packager::Deinitialize(PluginHost::IShell* service VARIABLE_IS_NOT_USED)
    {
        if (_service != nullptr) {
            ASSERT(_service == service);

            _service->Unregister(&_notification);

            if (_implementation != nullptr) {

                Exchange::JPackager::Unregister(*this);

                RPC::IRemoteConnection* connection(_service->RemoteConnection(_connectionId));

                VARIABLE_IS_NOT_USED uint32_t result = _implementation->Release();
                _implementation = nullptr;
                // It should have been the last reference we are releasing,
                // so it should end up in a DESCRUCTION_SUCCEEDED, if not we
                // are leaking ...
                ASSERT( (result == Core::ERROR_CONNECTION_CLOSED) || (result == Core::ERROR_DESTRUCTION_SUCCEEDED));

                // If this was running in a (container) process ...
                if (connection != nullptr) {
                    // Lets trigger the cleanup sequence for
                    // out-of-process code. Will will guard
                    // that unwilling processes, get shot if
                    // not stopped friendly :~)
                    connection->Terminate();
                    connection->Release();
                }
            }
            _service->Release();
            _service = nullptr;
            _connectionId = 0;
        }
    }

    string Packager::Information() const
    {
        return (string());
    }

    void Packager::Inbound(VARIABLE_IS_NOT_USED Web::Request& request)
    {
    }

    Core::ProxyType<Web::Response> Packager::Process(const Web::Request& request)
    {
        ASSERT(_skipURL <= request.Path.length());

        Core::ProxyType<Web::Response> result(PluginHost::IFactories::Instance().Response());
        Core::TextSegmentIterator index(Core::TextFragment(request.Path, _skipURL, request.Path.length() - _skipURL), false, '/');

        // Always skip the first one, it is an empty part because we start with a '/' if there are more parameters.
        index.Next();

        result->ErrorCode = Web::STATUS_BAD_REQUEST;
        result->Message = _T("Invalid request to packager plugin.");

        if (index.Next() && (request.Verb == Web::Request::HTTP_POST || request.Verb == Web::Request::HTTP_PUT)) {
            uint32_t status = Core::ERROR_UNAVAILABLE;
            if (index.Current().Text() == "Install") {
                std::array<char, kMaxValueLength> package {0};
                std::array<char, kMaxValueLength> version {0};
                std::array<char, kMaxValueLength> arch {0};
                Core::URL::KeyValue options(request.Query.Value());
                if (options.HasKey(_T("Package"), true) != Core::URL::KeyValue::status::UNAVAILABLE) {
                    const string name (options[_T("Package")].Text());
                    Core::URL::Decode (name.c_str(), name.length(), package.data(), package.size());
                }
                if (options.HasKey(_T("Architecture"), true) != Core::URL::KeyValue::status::UNAVAILABLE) {
                                    const string name (options[_T("Architecture")].Text());
                                    Core::URL::Decode (name.c_str(), name.length(), arch.data(), arch.size());
                }
                if (options.HasKey(_T("Version"), true) != Core::URL::KeyValue::status::UNAVAILABLE) {
                    const string name (options[_T("Version")].Text());
                    Core::URL::Decode (name.c_str(), name.length(), version.data(), version.size());
                }

                status = _implementation->Install(package.data(), version.data(), arch.data());
            } else if (index.Current().Text() == "SynchronizeRepository") {
                status = _implementation->SynchronizeRepository();
            }

            if (status == Core::ERROR_NONE) {
                result->ErrorCode = Web::STATUS_OK;
                result->Message = _T("OK");
            } else if (status == Core::ERROR_INPROGRESS) {
                result->Message = _T("Some operation already in progress. Only one at a time is allowed");
            }
        }

        return(result);
    }

    void Packager::Deactivated(RPC::IRemoteConnection* connection)
    {
        ASSERT(connection != nullptr);
        if (connection->Id() == _connectionId) {
            ASSERT(_service != nullptr);
            Core::IWorkerPool::Instance().Submit(PluginHost::IShell::Job::Create(_service,
                PluginHost::IShell::DEACTIVATED,
                PluginHost::IShell::FAILURE));
        }
    }
}  // namespace Plugin
}  // namespace Thunder
