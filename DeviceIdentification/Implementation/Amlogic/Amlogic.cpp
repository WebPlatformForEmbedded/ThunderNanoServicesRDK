#include "../../Module.h"

#include <fstream>

namespace Thunder {
namespace Plugin {
    class DeviceImplementation : public PluginHost::ISubSystem::IIdentifier {
        static constexpr const TCHAR* ChipsetInfo= _T("T962X3");
        static constexpr const TCHAR* VERSIONFile = _T("/version.txt");
        static constexpr const TCHAR* CMDLineFile = _T("/proc/cmdline");

    public:
        DeviceImplementation()
            : _chipset()
            , _firmwareVersion()
            , _identity()
        {
            UpdateChipset(_chipset);
            UpdateFirmwareVersion(_firmwareVersion);
            UpdateIdentifier();
        }

        DeviceImplementation(const DeviceImplementation&) = delete;
        DeviceImplementation& operator=(const DeviceImplementation&) = delete;

        virtual ~DeviceImplementation()
        {
            /* Nothing to do here. */
        }

    public:
        // IIdentifier interface
        uint8_t Identifier(const uint8_t length, uint8_t buffer[]) const override
        {
            uint8_t result = 0;
            if ((_identity.length())) {
                result = (_identity.length() > length ? length : _identity.length());
                ::memcpy(buffer, _identity.c_str(), result);
            } else {
                SYSLOG(Logging::Notification, (_T("Cannot determine system identity")));
            }

            return result;
        }
        string Architecture() const override
        {
            return Core::SystemInfo::Instance().Architecture();
        }
        string Chipset() const override
        {
            return _chipset;
        }
        string FirmwareVersion() const override
        {
            return _firmwareVersion;
        }

        BEGIN_INTERFACE_MAP(DeviceImplementation)
        INTERFACE_ENTRY(PluginHost::ISubSystem::IIdentifier)
        END_INTERFACE_MAP

    private:
        inline void UpdateFirmwareVersion(string& firmwareVersion) const
        {
            string line;
            std::ifstream file(VERSIONFile);

            if (file.is_open()) {
                while (getline(file, line)) {
                    if (line.find("SDK_VERSION") != std::string::npos) {
                        std::size_t position = line.find('=');
                        if (position != std::string::npos) {
                            firmwareVersion.assign(line.substr(position + 1, string::npos));
                            break;
                        }
                    }
                }

                file.close();
            }
        }

        inline void UpdateChipset(string& chipset) const
        {

#ifndef DEVICE_IDENTIFICATION_CHIPSET_INFO
             chipset.assign(ChipsetInfo);
#else
             chipset.assign(DEVICE_IDENTIFICATION_CHIPSET_INFO);
#endif
        }

        inline void UpdateIdentifier()
        {
            string line;
            std::ifstream file(CMDLineFile);
            if (file.is_open()) {
                while (getline(file, line)) {
                    std::size_t position;
                    if ((position = line.find("serialno")) != std::string::npos) {
                        position = line.find("=", position);
                        if (position != std::string::npos) {
                            std::size_t begin = position + 2;
                            std::size_t end = line.find(' ', begin);
                            _identity.assign(line.substr(begin, (end - begin)));
                            break;
                        }
                    }
                }
                file.close();
            }
            if (_identity.empty() == true) {
                TRACE(Trace::Error, (_T("There is no any valid identifier available")));
            }
        }

    private:
        string _chipset;
        string _firmwareVersion;
        string _identity;
    };

    SERVICE_REGISTRATION(DeviceImplementation, 1, 0)
}
}
