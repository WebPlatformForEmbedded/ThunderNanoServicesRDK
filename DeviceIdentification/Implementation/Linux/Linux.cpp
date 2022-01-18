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

#include "../../Module.h"

#include <fstream>
#include <sys/utsname.h>

namespace WPEFramework {
namespace Plugin {
    class DeviceImplementation : public PluginHost::ISubSystem::IIdentifier, public PluginHost::IConfigure {
    private:
        class Config : public Core::JSON::Container {
        public:
            Config(const Config&);
            Config& operator=(const Config&);

            Config()
                : Core::JSON::Container()
                , Interface()
            {
                Add(_T("interface"), &Interface);
            }

            ~Config() override
            {
            }

        public:
            Core::JSON::String Interface;
        };

    public:
        DeviceImplementation() = default;
        virtual ~DeviceImplementation() = default;

        DeviceImplementation(const DeviceImplementation&) = delete;
        DeviceImplementation& operator=(const DeviceImplementation&) = delete;

    public:
        uint32_t Configure(PluginHost::IShell* service) override
        {
            if (service) {
                Config config;
                config.FromString(service->ConfigLine());

                _interface = config.Interface.Value();
                if (_interface.empty() != true) {
                         UpdateDeviceId();
                }
            }

            return Core::ERROR_NONE;
        }

        uint8_t Identifier(const uint8_t length, uint8_t* buffer) const override
        {
            uint8_t ret = 0;
            if (_identifier.length()) {
                ret = (_identifier.length() > length ? length : _identifier.length());
                ::memcpy(buffer, _identifier.c_str(), ret);
            }
            return ret;
        }
        string Architecture() const override
        {
            return Core::SystemInfo::Instance().Architecture();
        }
        string Chipset() const override
        {
            return Core::SystemInfo::Instance().Chipset();
        }
        string FirmwareVersion() const override
        {
            return Core::SystemInfo::Instance().FirmwareVersion();
        }
    private:
        void UpdateDeviceId()
        {
            uint8_t MACAddressBuffer[6];

            Core::AdapterIterator adapter(_interface);

            if (adapter.IsValid() == true) {
                adapter.MACAddress(&MACAddressBuffer[0], sizeof(MACAddressBuffer));
                _identifier.assign(reinterpret_cast<char*>(MACAddressBuffer), sizeof(MACAddressBuffer));
            }
        }

    public:
        BEGIN_INTERFACE_MAP(DeviceImplementation)
        INTERFACE_ENTRY(PluginHost::ISubSystem::IIdentifier)
        INTERFACE_ENTRY(PluginHost::IConfigure)
        END_INTERFACE_MAP

    private:
        string _interface;
        string _identifier;
    };

    SERVICE_REGISTRATION(DeviceImplementation, 1, 0);

}
}
