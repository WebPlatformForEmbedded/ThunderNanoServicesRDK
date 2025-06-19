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

#pragma once

#include "Module.h"
#include <interfaces/IPluginAsyncStateControl.h>

namespace Thunder {
namespace Plugin {
    
    class PluginInitializerService : public PluginHost::IPlugin, public Exchange::IPluginAsyncStateControl {
    public:
        PluginInitializerService(const PluginInitializerService&) = delete;
        PluginInitializerService& operator=(const PluginInitializerService&) = delete;
        PluginInitializerService(PluginInitializerService&&) = delete;
        PluginInitializerService& operator=(PluginInitializerService&&) = delete;
        
        PluginInitializerService()
            : PluginHost::IPlugin()
            , Exchange::IPluginAsyncStateControl()
            , _example(0)
        {
        }
        
        ~PluginInitializerService() override = default;
    private:
        class Config : public Core::JSON::Container {
        public:
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;
            Config(Config&&) = delete;
            Config& operator=(Config&&) = delete;
            
            Config()
                : Core::JSON::Container()
                , Example()
            {
                Add(_T("example"), &Example);
            }
            ~Config() override = default;
        public:
            Core::JSON::String Example;
        };
        
    public:
        // IPlugin Methods
        const string Initialize(PluginHost::IShell* service) override;
        void Deinitialize(PluginHost::IShell* service) override;
        string Information() const override;
        
        // IPluginAsyncStateControl methods
        Core::hresult IPluginAsyncStateControlExample() override;
        
        // Plugin Methods
        void PluginInitializerServiceMethod();
        
        BEGIN_INTERFACE_MAP(PluginInitializerService)
            INTERFACE_ENTRY(PluginHost::IPlugin)
            INTERFACE_ENTRY(Exchange::IPluginAsyncStateControl)
        END_INTERFACE_MAP
        
    private:
        
        // Include the correct member variables for your plugin:
        // Note this is only an example, you are responsible for adding the correct members:
        uint32_t _example;
        
    };
} // Plugin
} // Thunder