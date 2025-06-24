/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2025 [PLEASE ADD COPYRIGHT NAME!]
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

#include "PluginInitializerService.h"

namespace Thunder {
namespace Plugin {
    
    namespace {
        
        static Metadata<PluginInitializerService>metadata(
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
    
    // Implement all methods from PluginInitializerService.h
    
    const string PluginInitializerService::Initialize(PluginHost::IShell* service) {
        string message;
        
        ASSERT(service != nullptr);
        ASSERT(_service == nullptr);

        Config config;
        config.FromString(service->ConfigLine());
        const Exchange::Controller::IMetadata* metadata = service->QueryInterfaceByCallsign<Exchange::Controller::IMetadata>(_T(""));
        ASSERT(metadata != nullptr);

        Exchange::Controller::IMetadata::Data::BuildInfo buildinfo{};
        Core::hresult result = metadata->BuildInfo(buildinfo);
        metadata->Release();
        metadata = nullptr;
        ASSERT(result == Core::ERROR_NONE);

        if (config.MaxParallel.IsSet() == true) {
            _maxparallel = config.MaxParallel.Value();
            if ((_maxparallel == 0) || (_maxparallel > (buildinfo.ThreadPoolCount-1))) {
                message = _T("maxparallel configured incorreclty");
            }
        } else {
            _maxparallel = ((buildinfo.ThreadPoolCount / 2) > 0 ? (buildinfo.ThreadPoolCount / 2) : 1 );
        }
        _maxretries = config.MaxRetries.Value();
        _delay = config.Delay.Value();

        if (message.empty() == true) {
            service->Register(&_sink);
            _service = service;
            _service->AddRef();
        }

        return (message);
    }
    
    void PluginInitializerService::Deinitialize(PluginHost::IShell* service) {

        if (_service != nullptr) {
            _service->Unregister(&_sink);
            _service->Release();
            _service = nullptr;
        }
    }
    
    string PluginInitializerService::Information() const {
        return (string());
    }

    Core::hresult PluginInitializerService::Activate(const string& callsign, const Core::OptionalType<uint8_t>& maxnumberretries, const Core::OptionalType<uint16_t>& delay, IActivationCallback* const cb)
    {
        Core::hresult result = Core::ERROR_NONE;
        ASSERT(cb != nullptr);

        PluginHost::IShell* requestedpluginShell = _service->QueryInterfaceByCallsign<PluginHost::IShell>(callsign);

        if (requestedpluginShell != nullptr) {
            PluginHost::IShell::state state = requestedpluginShell->State();
            if ((state == PluginHost::IShell::DEACTIVATED) || (state == PluginHost::IShell::DEACTIVATION)) {

            } else if ((state == PluginHost::IShell::ACTIVATED) || 
                       (state == PluginHost::IShell::ACTIVATION) ||
                       (state == PluginHost::IShell::PRECONDITION) || 
                       (state == PluginHost::IShell::HIBERNATED)) {
                requestedpluginShell->Release();
                requestedpluginShell = nullptr;
                if (cb != nullptr) {
                    cb->Finished(callsign, Exchange::IPluginAsyncStateControl::IActivationCallback::state::SUCCESS, 0);
                }
            } else {
                result = Core::ERROR_ILLEGAL_STATE;
                requestedpluginShell->Release();
                requestedpluginShell = nullptr;
            }
        } else {
            result = Core::ERROR_NOT_EXIST;        
        }

        return result;
    }
    Core::hresult PluginInitializerService::AbortActivate(const string& callsign)
    {
        return Core::ERROR_NONE;
    }
    
} // Plugin
} // Thunder