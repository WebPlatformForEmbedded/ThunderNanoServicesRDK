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

#include "DisplayInfo.h"

namespace Thunder {
namespace Plugin {

    namespace {

        static Metadata<DisplayInfo> metadata(
            // Version
            1, 0, 0,
            // Preconditions
            { subsystem::PLATFORM },
            // Terminations
            {},
            // Controls
            {}
        );
    }

    /* virtual */ const string DisplayInfo::Initialize(PluginHost::IShell* service)
    {
        string message;

        ASSERT(service != nullptr);
        ASSERT(_service == nullptr);
        ASSERT(_connectionProperties == nullptr);
        ASSERT(_connectionId == 0);
        ASSERT(_graphicsProperties == nullptr);
        ASSERT(_hdrProperties == nullptr);

        _service = service;
        _service->AddRef();
        _service->Register(&_notification);
        _skipURL = static_cast<uint8_t>(service->WebPrefix().length());

        _connectionProperties = service->Root<Exchange::IConnectionProperties>(_connectionId, 2000, _T("DisplayInfoImplementation"));
        if (_connectionProperties != nullptr) {
            _connectionProperties->Register(&_notification);
            Exchange::JConnectionProperties::Register(*this, _connectionProperties);

            Exchange::IConfiguration* configConnection = _connectionProperties->QueryInterface<Exchange::IConfiguration>();
            if (configConnection != nullptr) {
                configConnection->Configure(service);
                configConnection->Release();
            }

            _graphicsProperties = _connectionProperties->QueryInterface<Exchange::IGraphicsProperties>();
            if (_graphicsProperties == nullptr) {
                message = _T("DisplayInfo could not be instantiated. Could not acquire GraphicsProperties interface");
            } else {
                Exchange::JGraphicsProperties::Register(*this, _graphicsProperties);
                _hdrProperties = _connectionProperties->QueryInterface<Exchange::IHDRProperties>();
                if (_hdrProperties == nullptr) {
                    message = _T("DisplayInfo could not be instantiated. Could not acquire HDRProperties interface");
                } else {
                    Exchange::JHDRProperties::Register(*this, _hdrProperties);

                    // The code execution should proceed regardless of the _displayProperties
                    // value, as it is not a essential.
                    // The relevant JSONRPC endpoints will return ERROR_UNAVAILABLE,
                    // if it hasn't been initialized.
                    _displayProperties = _connectionProperties->QueryInterface<Exchange::IDisplayProperties>();
                    if (_displayProperties == nullptr) {
                        SYSLOG(Logging::Startup, (_T("Display Properties service is unavailable.")));
                    } else {
                        Exchange::JDisplayProperties::Register(*this, _displayProperties);
                    }
                }
            }
        } else {
            message = _T("DisplayInfo could not be instantiated. Could not acquire ConnectionProperties interface");
        }

        return message;
    }

    void DisplayInfo::Deinitialize(PluginHost::IShell* service) /* override */
    {
        if (_service != nullptr) {
            ASSERT(service == _service);

            _service->Unregister(&_notification);

            if (_connectionProperties != nullptr) {
                _connectionProperties->Unregister(&_notification);
                Exchange::JConnectionProperties::Unregister(*this);

                if (_hdrProperties != nullptr) {
                    Exchange::JHDRProperties::Unregister(*this);
                    _hdrProperties->Release();
                    _hdrProperties = nullptr;
                }

                if (_graphicsProperties != nullptr) {
                    Exchange::JGraphicsProperties::Unregister(*this);
                    _graphicsProperties->Release();
                    _graphicsProperties = nullptr;
                }

                if (_displayProperties != nullptr) {
                    _displayProperties->Release();
                    Exchange::JDisplayProperties::Unregister(*this);
                    _displayProperties = nullptr;
                }

                // Stop processing:
                RPC::IRemoteConnection* connection = service->RemoteConnection(_connectionId);
                VARIABLE_IS_NOT_USED uint32_t result = _connectionProperties->Release();
                _connectionProperties = nullptr;

                // It should have been the last reference we are releasing, 
                // so it should endup in a DESTRUCTION_SUCCEEDED, if not we
                // are leaking...
                ASSERT(result == Core::ERROR_DESTRUCTION_SUCCEEDED);

                // If this was running in a (container) process...
                if (connection != nullptr) {
                    // Lets trigger the cleanup sequence for
                    // out-of-process code. Which will guard
                    // that unwilling processes, get shot if
                    // not stopped friendly :-)
                    connection->Terminate();
                    connection->Release();
                }
            }
            _connectionId = 0;
            _service->Release();
            _service = nullptr;
        }
    }

    string DisplayInfo::Information() const /* override */
    {
        // No additional info to report.
        return (string());
    }

    void DisplayInfo::Deactivated(RPC::IRemoteConnection* connection)
    {
        // This can potentially be called on a socket thread, so the deactivation (wich in turn kills this object) must be done
        // on a seperate thread. Also make sure this call-stack can be unwound before we are totally destructed.
        if (_connectionId == connection->Id()) {

            ASSERT(_service != nullptr);

            Core::IWorkerPool::Instance().Submit(PluginHost::IShell::Job::Create(_service, PluginHost::IShell::DEACTIVATED, PluginHost::IShell::FAILURE));
        }
    }

} // namespace Plugin
} // namespace Thunder
