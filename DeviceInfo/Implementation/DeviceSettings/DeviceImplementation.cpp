#include "DeviceInfoImplementation.h"

#include "mfrMgr.h"
#include "rfcapi.h"
#include "exception.hpp"
#include "host.hpp"
#include "manager.hpp"
#include "videoOutputPortConfig.hpp"

#include "UtilsIarm.h"

#include <fstream>
#include <regex>

namespace WPEFramework {
namespace Plugin {
    namespace {
        uint32_t GetFileRegex(const char* filename, const std::regex& regex, string& response)
        {
            uint32_t result = Core::ERROR_GENERAL;

            std::ifstream file(filename);
            if (file) {
                string line;
                while (std::getline(file, line)) {
                    std::smatch sm;
                    if (std::regex_match(line, sm, regex)) {
                        ASSERT(sm.size() == 2);
                        response = sm[1];
                        result = Core::ERROR_NONE;
                        break;
                    }
                }
            }

            return result;
        }

        uint32_t GetMFRData(mfrSerializedType_t type, string& response)
        {
            uint32_t result = Core::ERROR_GENERAL;

            IARM_Bus_MFRLib_GetSerializedData_Param_t param;
            param.bufLen = 0;
            param.type = type;
            auto status = IARM_Bus_Call(
                IARM_BUS_MFRLIB_NAME, IARM_BUS_MFRLIB_API_GetSerializedData, &param, sizeof(param));
            if ((status == IARM_RESULT_SUCCESS) && param.bufLen) {
                response.assign(param.buffer, param.bufLen);
                result = Core::ERROR_NONE;
            } else {
                TRACE_GLOBAL(Trace::Information, (_T("MFR error [%d] for %d"), status, type));
            }

            return result;
        }

        uint32_t GetRFCData(const char* name, string& response)
        {
            uint32_t result = Core::ERROR_GENERAL;

            RFC_ParamData_t param;
            auto status = getRFCParameter(nullptr, name, &param);
            if ((status == WDMP_SUCCESS) && param.value[0]) {
                response = param.value;
                result = Core::ERROR_NONE;
            } else {
                TRACE_GLOBAL(Trace::Information, (_T("RFC error [%d] for %s"), status, name));
            }

            return result;
        }
    }

    SERVICE_REGISTRATION(DeviceInfoImplementation, 1, 0);

    DeviceInfoImplementation::DeviceInfoImplementation()
    {
        Utils::IARM::init();
        try {
            device::Manager::Initialize();
        } catch (const device::Exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
        } catch (const std::exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
        } catch (...) {
        }
    }

    uint32_t DeviceInfoImplementation::SerialNumber(string& serialNumber) const
    {
        return (GetMFRData(mfrSERIALIZED_TYPE_SERIALNUMBER, serialNumber)
                   == Core::ERROR_NONE)
            ? Core::ERROR_NONE
            : GetRFCData(_T("Device.DeviceInfo.SerialNumber"), serialNumber);
    }

    uint32_t DeviceInfoImplementation::Sku(string& sku) const
    {
        return (GetFileRegex(_T("/etc/device.properties"),
                    std::regex("^MODEL_NUM(?:\\s*)=(?:\\s*)(?:\"{0,1})([^\"\\n]+)(?:\"{0,1})(?:\\s*)$"), sku)
                   == Core::ERROR_NONE)
            ? Core::ERROR_NONE
            : ((GetMFRData(mfrSERIALIZED_TYPE_MODELNAME, sku)
                   == Core::ERROR_NONE)
                    ? Core::ERROR_NONE
                    : GetRFCData(_T("Device.DeviceInfo.ModelName"), sku));
    }

    uint32_t DeviceInfoImplementation::Make(string& make) const
    {
        return (GetFileRegex(_T("/etc/device.properties"),
                    std::regex("^MFG_NAME(?:\\s*)=(?:\\s*)(?:\"{0,1})([^\"\\n]+)(?:\"{0,1})(?:\\s*)$"), make)
                   == Core::ERROR_NONE)
            ? Core::ERROR_NONE
            : GetMFRData(mfrSERIALIZED_TYPE_MANUFACTURER, make);
    }

    uint32_t DeviceInfoImplementation::Model(string& model) const
    {
        return
        // TYPE_SKYMODELNAME exists for oem-sky-realtek
#ifdef ENABLE_DEVICE_MANUFACTURER_INFO
            (GetMFRData(mfrSERIALIZED_TYPE_PROVISIONED_MODELNAME, model) == Core::ERROR_NONE)
            ? Core::ERROR_NONE
            :
#endif
            GetFileRegex(_T("/etc/device.properties"),
                std::regex("^FRIENDLY_ID(?:\\s*)=(?:\\s*)(?:\"{0,1})([^\"\\n]+)(?:\"{0,1})(?:\\s*)$"), model);
    }

    uint32_t DeviceInfoImplementation::DeviceType(string& deviceType) const
    {
#ifndef ENABLE_COMMUNITY_DEVICE_TYPE
        return GetFileRegex(_T("/etc/authService.conf"),std::regex("^deviceType(?:\\s*)=(?:\\s*)(?:\"{0,1})([^\"\\n]+)(?:\"{0,1})(?:\\s*)$"), deviceType);
#else
        const char* device_type;
        uint32_t result = GetFileRegex(_T("/etc/authService.conf"),
            std::regex("^deviceType(?:\\s*)=(?:\\s*)(?:\"{0,1})([^\"\\n]+)(?:\"{0,1})(?:\\s*)$"), deviceType)
            == Core::ERROR_NONE ? Core::ERROR_NONE
            : GetFileRegex(_T("/etc/device.properties"),
                                        std::regex("^DEVICE_TYPE(?:\\s*)=(?:\\s*)(?:\"{0,1})([^\"\\n]+)(?:\"{0,1})(?:\\s*)$"), deviceType);
        device_type = deviceType.c_str();
        deviceType = (strcmp("mediaclient",device_type)==0)?("IpStb"):((strcmp("hybrid",device_type)==0)?("QamIpStb"):("TV"));
        return result;
#endif
    }

    uint32_t DeviceInfoImplementation::DistributorId(string& distributorId) const
    {
        return (GetFileRegex(_T("/opt/www/authService/partnerId3.dat"),
                    std::regex("^([^\\n]+)$"), distributorId)
                   == Core::ERROR_NONE)
            ? Core::ERROR_NONE
            : GetRFCData(_T("Device.DeviceInfo.X_RDKCENTRAL-COM_Syndication.PartnerId"), distributorId);
    }

    uint32_t DeviceInfoImplementation::SupportedAudioPorts(RPC::IStringIterator*& supportedAudioPorts) const
    {
        uint32_t result = Core::ERROR_NONE;

        std::list<string> list;

        try {
            const auto& aPorts = device::Host::getInstance().getAudioOutputPorts();
            for (size_t i = 0; i < aPorts.size(); i++) {
                list.emplace_back(aPorts.at(i).getName());
            }
        } catch (const device::Exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (const std::exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (...) {
            result = Core::ERROR_GENERAL;
        }

        if (result == Core::ERROR_NONE) {
            supportedAudioPorts = (Core::Service<RPC::StringIterator>::Create<RPC::IStringIterator>(list));
        }

        return result;
    }

    uint32_t DeviceInfoImplementation::AudioCapabilities(const string& audioPort, Exchange::IDeviceAudioCapabilities::IAudioCapabilityIterator*& audioCapabilities) const
    {
        uint32_t result = Core::ERROR_NONE;

        std::list<Exchange::IDeviceAudioCapabilities::AudioCapability> list;

        int capabilities = dsAUDIOSUPPORT_NONE;

        try {
            auto strAudioPort = audioPort.empty() ? device::Host::getInstance().getDefaultAudioPortName() : audioPort;
            auto& aPort = device::Host::getInstance().getAudioOutputPort(strAudioPort);
            aPort.getAudioCapabilities(&capabilities);
        } catch (const device::Exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (const std::exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (...) {
            result = Core::ERROR_GENERAL;
        }

        if (result == Core::ERROR_NONE) {
            supportedAudioPorts = (Core::Service<RPC::StringIterator>::Create<RPC::IStringIterator>(list));
        }

        return result;
    }

    uint32_t DeviceInfoImplementation::AudioCapabilities(const string& audioPort, Exchange::IDeviceAudioCapabilities::IAudioCapabilityIterator*& audioCapabilities) const
    {
        uint32_t result = Core::ERROR_NONE;

        std::list<Exchange::IDeviceAudioCapabilities::AudioCapability> list;

        int capabilities = dsAUDIOSUPPORT_NONE;

        try {
            auto strAudioPort = audioPort.empty() ? device::Host::getInstance().getDefaultAudioPortName() : audioPort;
            auto& aPort = device::Host::getInstance().getAudioOutputPort(strAudioPort);
            aPort.getAudioCapabilities(&capabilities);
        } catch (const device::Exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (const std::exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (...) {
            result = Core::ERROR_GENERAL;
        }

        if (!capabilities)
            list.emplace_back(Exchange::IDeviceAudioCapabilities::AudioCapability::AUDIOCAPABILITY_NONE);
        if (capabilities & dsAUDIOSUPPORT_ATMOS)
            list.emplace_back(Exchange::IDeviceAudioCapabilities::AudioCapability::ATMOS);
        if (capabilities & dsAUDIOSUPPORT_DD)
            list.emplace_back(Exchange::IDeviceAudioCapabilities::AudioCapability::DD);
        if (capabilities & dsAUDIOSUPPORT_DDPLUS)
            list.emplace_back(Exchange::IDeviceAudioCapabilities::AudioCapability::DDPLUS);
        if (capabilities & dsAUDIOSUPPORT_DAD)
            list.emplace_back(Exchange::IDeviceAudioCapabilities::AudioCapability::DAD);
        if (capabilities & dsAUDIOSUPPORT_DAPv2)
            list.emplace_back(Exchange::IDeviceAudioCapabilities::AudioCapability::DAPV2);
        if (capabilities & dsAUDIOSUPPORT_MS12)
            list.emplace_back(Exchange::IDeviceAudioCapabilities::AudioCapability::MS12);

        if (result == Core::ERROR_NONE) {
            audioCapabilities = (Core::Service<RPC::IteratorType<Exchange::IDeviceAudioCapabilities::IAudioCapabilityIterator>>::Create<Exchange::IDeviceAudioCapabilities::IAudioCapabilityIterator>(list));
        }

        return result;
    }

    uint32_t DeviceInfoImplementation::MS12Capabilities(const string& audioPort, Exchange::IDeviceAudioCapabilities::IMS12CapabilityIterator*& ms12Capabilities) const
    {
        uint32_t result = Core::ERROR_NONE;

        std::list<Exchange::IDeviceAudioCapabilities::MS12Capability> list;

        int capabilities = dsMS12SUPPORT_NONE;

        try {
            auto strAudioPort = audioPort.empty() ? device::Host::getInstance().getDefaultAudioPortName() : audioPort;
            auto& aPort = device::Host::getInstance().getAudioOutputPort(strAudioPort);
            aPort.getMS12Capabilities(&capabilities);
        } catch (const device::Exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (const std::exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (...) {
            result = Core::ERROR_GENERAL;
        }

        if (!capabilities)
            list.emplace_back(Exchange::IDeviceAudioCapabilities::MS12Capability::MS12CAPABILITY_NONE);
        if (capabilities & dsMS12SUPPORT_DolbyVolume)
            list.emplace_back(Exchange::IDeviceAudioCapabilities::MS12Capability::DOLBYVOLUME);
        if (capabilities & dsMS12SUPPORT_InteligentEqualizer)
            list.emplace_back(Exchange::IDeviceAudioCapabilities::MS12Capability::INTELIGENTEQUALIZER);
        if (capabilities & dsMS12SUPPORT_DialogueEnhancer)
            list.emplace_back(Exchange::IDeviceAudioCapabilities::MS12Capability::DIALOGUEENHANCER);

        if (result == Core::ERROR_NONE) {
            ms12Capabilities = (Core::Service<RPC::IteratorType<Exchange::IDeviceAudioCapabilities::IMS12CapabilityIterator>>::Create<Exchange::IDeviceAudioCapabilities::IMS12CapabilityIterator>(list));
        }

        return result;
    }

    uint32_t DeviceInfoImplementation::SupportedMS12AudioProfiles(const string& audioPort, RPC::IStringIterator*& supportedMS12AudioProfiles) const
    {
        uint32_t result = Core::ERROR_NONE;

        std::list<string> list;

        try {
            auto strAudioPort = audioPort.empty() ? device::Host::getInstance().getDefaultAudioPortName() : audioPort;
            auto& aPort = device::Host::getInstance().getAudioOutputPort(strAudioPort);
            const auto supportedProfiles = aPort.getMS12AudioProfileList();
            for (size_t i = 0; i < supportedProfiles.size(); i++) {
                list.emplace_back(supportedProfiles.at(i));
            }
        } catch (const device::Exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (const std::exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (...) {
            result = Core::ERROR_GENERAL;
        }

        if (result == Core::ERROR_NONE) {
            supportedMS12AudioProfiles = (Core::Service<RPC::StringIterator>::Create<RPC::IStringIterator>(list));
        }

        return result;
    }

    uint32_t DeviceInfoImplementation::SupportedVideoDisplays(RPC::IStringIterator*& supportedVideoDisplays) const
    {
        uint32_t result = Core::ERROR_NONE;

        std::list<string> list;

        try {
            const auto& vPorts = device::Host::getInstance().getVideoOutputPorts();
            for (size_t i = 0; i < vPorts.size(); i++) {

                /**
                 * There's N:1 relation between VideoOutputPort and AudioOutputPort.
                 * When there are multiple Audio Ports on the Video Port,
                 * there are multiple VideoOutputPort-s as well.
                 * Those VideoOutputPort-s are the same except holding a different Audio Port id.
                 * As a result, a list of Video Ports has multiple Video Ports
                 * that represent the same Video Port, but different Audio Port.
                 * A list of VideoOutputPort-s returned from DS
                 * needs to be filtered by name.
                 */

                auto name = vPorts.at(i).getName();
                if (std::find(list.begin(), list.end(), name) != list.end())
                    continue;

                list.emplace_back(name);
            }
        } catch (const device::Exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (const std::exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (...) {
            result = Core::ERROR_GENERAL;
        }

        if (result == Core::ERROR_NONE) {
            supportedVideoDisplays = (Core::Service<RPC::StringIterator>::Create<RPC::IStringIterator>(list));
        }

        return result;
    }

    uint32_t DeviceInfoImplementation::HostEDID(string& edid) const
    {
        uint32_t result = Core::ERROR_NONE;

        std::vector<uint8_t> edidVec({ 'u', 'n', 'k', 'n', 'o', 'w', 'n' });
        try {
            std::vector<unsigned char> edidVec2;
            device::Host::getInstance().getHostEDID(edidVec2);
            edidVec = edidVec2;
        } catch (const device::Exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (const std::exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (...) {
            result = Core::ERROR_GENERAL;
        }

        if (result == Core::ERROR_NONE) {
            // convert to base64

            if (edidVec.size() > (size_t)std::numeric_limits<uint16_t>::max()) {
                result = Core::ERROR_GENERAL;
            } else {
                string base64String;
                Core::ToString((uint8_t*)&edidVec[0], edidVec.size(), true, base64String);
                edid = base64String;
            }
        }

        return result;
    }

    uint32_t DeviceInfoImplementation::DefaultResolution(const string& videoDisplay, string& defaultResolution) const
    {
        uint32_t result = Core::ERROR_NONE;

        try {
            auto strVideoPort = videoDisplay.empty() ? device::Host::getInstance().getDefaultVideoPortName() : videoDisplay;
            auto& vPort = device::Host::getInstance().getVideoOutputPort(strVideoPort);
            defaultResolution = vPort.getDefaultResolution().getName();
        } catch (const device::Exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (const std::exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (...) {
            result = Core::ERROR_GENERAL;
        }

        return result;
    }

    uint32_t DeviceInfoImplementation::SupportedResolutions(const string& videoDisplay, RPC::IStringIterator*& supportedResolutions) const
    {
        uint32_t result = Core::ERROR_NONE;

        std::list<string> list;

        try {
            auto strVideoPort = videoDisplay.empty() ? device::Host::getInstance().getDefaultVideoPortName() : videoDisplay;
            auto& vPort = device::Host::getInstance().getVideoOutputPort(strVideoPort);
            const auto resolutions = device::VideoOutputPortConfig::getInstance().getPortType(vPort.getType().getId()).getSupportedResolutions();
            for (size_t i = 0; i < resolutions.size(); i++) {
                list.emplace_back(resolutions.at(i).getName());
            }
        } catch (const device::Exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (const std::exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (...) {
            result = Core::ERROR_GENERAL;
        }

        if (result == Core::ERROR_NONE) {
            supportedResolutions = (Core::Service<RPC::StringIterator>::Create<RPC::IStringIterator>(list));
        }

        return result;
    }

    uint32_t DeviceInfoImplementation::SupportedHdcp(const string& videoDisplay, Exchange::IDeviceVideoCapabilities::CopyProtection& supportedHDCPVersion) const
    {
        uint32_t result = Core::ERROR_NONE;

        try {
            auto strVideoPort = videoDisplay.empty() ? device::Host::getInstance().getDefaultVideoPortName() : videoDisplay;
            auto& vPort = device::VideoOutputPortConfig::getInstance().getPort(strVideoPort);
            switch (vPort.getHDCPProtocol()) {
            case dsHDCP_VERSION_2X:
                supportedHDCPVersion = Exchange::IDeviceVideoCapabilities::CopyProtection::HDCP_22;
                break;
            case dsHDCP_VERSION_1X:
                supportedHDCPVersion = Exchange::IDeviceVideoCapabilities::CopyProtection::HDCP_14;
                break;
            default:
                result = Core::ERROR_GENERAL;
            }
        } catch (const device::Exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (const std::exception& e) {
            TRACE(Trace::Fatal, (_T("Exception caught %s"), e.what()));
            result = Core::ERROR_GENERAL;
        } catch (...) {
            result = Core::ERROR_GENERAL;
        }

        return result;
    }

    uint32_t DeviceInfoImplementation::Imagename(string& imagename) const
    {
        return GetFileRegex(_T("/version.txt"), std::regex("^imagename:([^\\n]+)$"), imagename);
    }

    uint32_t DeviceInfoImplementation::Sdk(string& sdk) const
    {
        return GetFileRegex(_T("/version.txt"), std::regex("^SDK_VERSION=([^\\n]+)$"), sdk);
    }

    uint32_t DeviceInfoImplementation::Mediarite(string& mediarite) const
    {
        return GetFileRegex(_T("/version.txt"), std::regex("^MEDIARITE=([^\\n]+)$"), mediarite);
    }

    uint32_t DeviceInfoImplementation::Yocto(string& yocto) const
    {
        return GetFileRegex(_T("/version.txt"), std::regex("^YOCTO_VERSION=([^\\n]+)$"), yocto);
    }

}
}
