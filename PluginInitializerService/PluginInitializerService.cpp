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
        Config config;
        config.FromString(service->ConfigLine());
        TRACE(Trace::Information, (_T("This is just an example: [%s])"), config.Example.Value().c_str()));
        return (message);
    }
    
    void PluginInitializerService::Deinitialize(VARIABLE_IS_NOT_USED PluginHost::IShell* service) {
    }
    
    string PluginInitializerService::Information() const {
        return (string());
    }

    Core::hresult PluginInitializerService::Activate(const string& callsign, const Core::OptionalType<uint8_t>& maxnumberretries, const Core::OptionalType<uint16_t>& delay, IActivationCallback* const cb)
    {
        ASSERT(cb != nullptr);
        if (cb != nullptr) {
            cb->Finished(callsign, Exchange::IPluginAsyncStateControl::IActivationCallback::state::FAILURE, 0);
        }
        return Core::ERROR_NONE;
    }
    Core::hresult PluginInitializerService::AbortActivate(const string& callsign)
    {
        return Core::ERROR_NONE;
    }
    
} // Plugin
} // Thunder