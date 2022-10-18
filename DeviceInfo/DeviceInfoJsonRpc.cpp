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

#include "Module.h"
#include "DeviceInfo.h"
#include <interfaces/json/JsonData_DeviceInfo.h>

namespace WPEFramework {

namespace Plugin {

    using namespace JsonData::DeviceInfo;

    void DeviceInfo::RegisterAll()
    {
        Register<SupportedresolutionsParamsInfo,SupportedresolutionsResultData>(_T("supportedresolutions"), &DeviceInfo::endpoint_supportedresolutions, this);
        Register<SupportedresolutionsParamsInfo,DefaultresolutionResultData>(_T("defaultresolution"), &DeviceInfo::endpoint_defaultresolution, this);
        Register<SupportedresolutionsParamsInfo,SupportedhdcpResultData>(_T("supportedhdcp"), &DeviceInfo::endpoint_supportedhdcp, this);
        Register<AudiocapabilitiesParamsInfo,AudiocapabilitiesResultData>(_T("audiocapabilities"), &DeviceInfo::endpoint_audiocapabilities, this);
        Register<AudiocapabilitiesParamsInfo,Ms12capabilitiesResultData>(_T("ms12capabilities"), &DeviceInfo::endpoint_ms12capabilities, this);
        Register<AudiocapabilitiesParamsInfo,Supportedms12audioprofilesResultData>(_T("supportedms12audioprofiles"), &DeviceInfo::endpoint_supportedms12audioprofiles, this);
        Property<DeviceaudiocapabilitiesData>(_T("deviceaudiocapabilities"), &DeviceInfo::endpoint_get_deviceaudiocapabilities, nullptr, this);
        Property<DevicevideocapabilitiesData>(_T("devicevideocapabilities"), &DeviceInfo::endpoint_get_devicevideocapabilities, nullptr, this);
        Property<DeviceinfoData>(_T("deviceinfo"), &DeviceInfo::endpoint_get_deviceinfo, nullptr, this);
        Property<SysteminfoData>(_T("systeminfo"), &DeviceInfo::endpoint_get_systeminfo, nullptr, this);
        Property<Core::JSON::ArrayType<AddressesData>>(_T("addresses"), &DeviceInfo::endpoint_get_addresses, nullptr, this);
        Property<SocketinfoData>(_T("socketinfo"), &DeviceInfo::endpoint_get_socketinfo, nullptr, this);
        Property<SupportedaudioportsData>(_T("supportedaudioports"), &DeviceInfo::endpoint_get_supportedaudioports, nullptr, this);
        Property<SupportedvideodisplaysData>(_T("supportedvideodisplays"), &DeviceInfo::endpoint_get_supportedvideodisplays, nullptr, this);
        Property<HostedidData>(_T("hostedid"), &DeviceInfo::endpoint_get_hostedid, nullptr, this);
        Property<FirmwareversionData>(_T("firmwareversion"), &DeviceInfo::endpoint_get_firmwareversion, nullptr, this);
        Property<SerialnumberData>(_T("serialnumber"), &DeviceInfo::endpoint_get_serialnumber, nullptr, this);
        Property<ModelidData>(_T("modelid"), &DeviceInfo::endpoint_get_modelid, nullptr, this);
        Property<MakeData>(_T("make"), &DeviceInfo::endpoint_get_make, nullptr, this);
        Property<ModelnameData>(_T("modelname"), &DeviceInfo::endpoint_get_modelname, nullptr, this);
        Property<ModelyearData>(_T("modelyear"), &DeviceInfo::endpoint_get_modelyear, nullptr, this);
        Property<FriendlynameInfo>(_T("friendlyname"), &DeviceInfo::endpoint_get_friendlyname, nullptr, this);
        Property<FriendlynameInfo>(_T("platformname"), &DeviceInfo::endpoint_get_platformname, nullptr, this);
        Property<DevicetypeData>(_T("devicetype"), &DeviceInfo::endpoint_get_devicetype, nullptr, this);
        Property<DistributoridData>(_T("distributorid"), &DeviceInfo::endpoint_get_distributorid, nullptr, this);
    }

    void DeviceInfo::UnregisterAll()
    {
        Unregister(_T("supportedms12audioprofiles"));
        Unregister(_T("ms12capabilities"));
        Unregister(_T("audiocapabilities"));
        Unregister(_T("supportedhdcp"));
        Unregister(_T("defaultresolution"));
        Unregister(_T("supportedresolutions"));
        Unregister(_T("distributorid"));
        Unregister(_T("devicetype"));
        Unregister(_T("platformname"));
        Unregister(_T("friendlyname"));
        Unregister(_T("modelyear"));
        Unregister(_T("modelname"));
        Unregister(_T("make"));
        Unregister(_T("modelid"));
        Unregister(_T("serialnumber"));
        Unregister(_T("firmwareversion"));
        Unregister(_T("hostedid"));
        Unregister(_T("supportedvideodisplays"));
        Unregister(_T("supportedaudioports"));
        Unregister(_T("socketinfo"));
        Unregister(_T("addresses"));
        Unregister(_T("systeminfo"));
        Unregister(_T("deviceinfo"));
        Unregister(_T("devicevideocapabilities"));
        Unregister(_T("deviceaudiocapabilities"));
    }

    // API implementation
    //

    // Method: supportedresolutions - Supported resolutions on the selected video display port
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_supportedresolutions(const SupportedresolutionsParamsInfo& params, SupportedresolutionsResultData& response)
    {
        const VideodisplayType& videooutput = params.VideoDisplay.Value();
        return Resolutions(static_cast<Exchange::IDeviceVideoCapabilities::VideoOutput>(videooutput), response.SupportedResolutions);
    }

    // Method: defaultresolution - Default resolution on the selected video display port
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_defaultresolution(const SupportedresolutionsParamsInfo& params, DefaultresolutionResultData& response)
    {
        const VideodisplayType& videooutput = params.VideoDisplay.Value();
        return DefaultResolution(static_cast<Exchange::IDeviceVideoCapabilities::VideoOutput>(videooutput), response.DefaultResolution);
    }

    // Method: supportedhdcp - Supported hdcp version on the selected video display port
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_supportedhdcp(const SupportedresolutionsParamsInfo& params, SupportedhdcpResultData& response)
    {
        const VideodisplayType& videooutput = params.VideoDisplay.Value();
        return Hdcp(static_cast<Exchange::IDeviceVideoCapabilities::VideoOutput>(videooutput), response.SupportedHDCPVersion);
    }

    // Method: audiocapabilities - Audio capabilities for the specified audio port
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_audiocapabilities(const AudiocapabilitiesParamsInfo& params, AudiocapabilitiesResultData& response)
    {
        const AudioportType& audiooutput = params.AudioPort.Value();
        return AudioCapabilities(static_cast<Exchange::IDeviceAudioCapabilities::AudioOutput>(audiooutput), response.AudioCapabilities);
    }

    // Method: ms12capabilities - Audio ms12 capabilities for the specified audio port
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_ms12capabilities(const AudiocapabilitiesParamsInfo& params, Ms12capabilitiesResultData& response)
    {
        const AudioportType& audiooutput = params.AudioPort.Value();
        return Ms12Capabilities(static_cast<Exchange::IDeviceAudioCapabilities::AudioOutput>(audiooutput), response.MS12Capabilities);
    }

    // Method: supportedms12audioprofiles - Supported ms12 audio profiles for the specified audio port
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_supportedms12audioprofiles(const AudiocapabilitiesParamsInfo& params, Supportedms12audioprofilesResultData& response)
    {
        const AudioportType& audiooutput = params.AudioPort.Value();
        return Ms12Profiles(static_cast<Exchange::IDeviceAudioCapabilities::AudioOutput>(audiooutput), response.SupportedMS12AudioProfiles);
    }

    // Property: deviceaudiocapabilities - Audio capabilities of the device
    // Return codes:
    //  - ERROR_NONE: Success
    uint32_t DeviceInfo::endpoint_get_deviceaudiocapabilities(DeviceaudiocapabilitiesData& response) const
    {
        AudioCapabilitiesInfo(response);
        return Core::ERROR_NONE;
    }

    // Property: devicevideocapabilities - Video capabilities of the device
    // Return codes:
    //  - ERROR_NONE: Success
    uint32_t DeviceInfo::endpoint_get_devicevideocapabilities(DevicevideocapabilitiesData& response) const
    {
        VideoCapabilitiesInfo(response);;
        return Core::ERROR_NONE;
    }

    // Property: deviceinfo - Device meta data
    // Return codes:
    //  - ERROR_NONE: Success
    uint32_t DeviceInfo::endpoint_get_deviceinfo(DeviceinfoData& response) const
    {
        DeviceMetaData(response);
        return Core::ERROR_NONE;
    }

    // Property: systeminfo - System general information
    // Return codes:
    //  - ERROR_NONE: Success
    uint32_t DeviceInfo::endpoint_get_systeminfo(SysteminfoData& response) const
    {
        SysInfo(response);
        return Core::ERROR_NONE;
    }

    // Property: addresses - Network interface addresses
    // Return codes:
    //  - ERROR_NONE: Success
    uint32_t DeviceInfo::endpoint_get_addresses(Core::JSON::ArrayType<AddressesData>& response) const
    {
        AddressInfo(response);
        return Core::ERROR_NONE;
    }

    // Property: socketinfo - Socket information
    // Return codes:
    //  - ERROR_NONE: Success
    uint32_t DeviceInfo::endpoint_get_socketinfo(SocketinfoData& response) const
    {
        SocketPortInfo(response);
        return Core::ERROR_NONE;
    }

    // Property: supportedaudioports - Audio ports supported on the device (all ports that are physically present)
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_supportedaudioports(SupportedaudioportsData& response) const
    {
        return AudioOutputs(response.SupportedAudioPorts);
    }

    // Property: supportedvideodisplays - Video ports supported on the device (all ports that are physically present)
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_supportedvideodisplays(SupportedvideodisplaysData& response) const
    {
        return VideoOutputs(response.SupportedVideoDisplays);
    }

    // Property: hostedid - EDID of the host
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_hostedid(HostedidData& response) const
    {
        return HostEDID(response.EDID);
    }

    // Property: firmwareversion - Versions maintained in version
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_UNAVAILABLE: FirmwareVersion information is not available
    uint32_t DeviceInfo::endpoint_get_firmwareversion(FirmwareversionData& response) const
    {
        return Core::ERROR_UNAVAILABLE;
    }

    // Property: serialnumber - Serial number set by manufacturer
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_serialnumber(SerialnumberData& response) const
    {
        uint32_t result = Core::ERROR_NONE;
        string serialNumber;
	result = _deviceInfo->SerialNumber(serialNumber);

	response.Serialnumber = serialNumber;
        return result;
    }

    // Property: modelid - Device model number or SKU
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_modelid(ModelidData& response) const
    {
        string sku;
	uint32_t result = _deviceInfo->Sku(sku);
        if (result == Core::ERROR_NONE) {
            Core::EnumerateType<JsonData::DeviceInfo::ModelidData::SkuType> value(sku.c_str(), false);
            if (value.IsSet()) {
                response.Sku = value.Value();
            } else {
                TRACE(Trace::Fatal, (_T("Unknown value %s"), sku.c_str()));
                result = Core::ERROR_GENERAL;
            }
        }

        return result;
    }

    // Property: make - Device manufacturer
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_make(MakeData& response) const
    {
	string make;
        uint32_t result = _deviceInfo->Make(make);
        if (result == Core::ERROR_NONE) {
            Core::EnumerateType<JsonData::DeviceInfo::MakeData::MakeType> value(make.c_str(), false);
            if (value.IsSet()) {
                response.Make = value.Value();
            } else {
                TRACE(Trace::Fatal, (_T("Unknown value %s"), make.c_str()));
                result = Core::ERROR_GENERAL;
            }
        }
        return result;
    }

    // Property: modelname - Friendly device model name
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_modelname(ModelnameData& response) const
    {
        string modelName;
        uint32_t result = _deviceInfo->ModelName(modelName);

        response.Model = modelName;
        return result;
    }

    // Property: modelyear - Friendly device model year
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_modelyear(ModelyearData& response) const
    {
        uint16_t year;
        uint32_t result = _deviceInfo->ModelYear(year);
	response.Year = year;

        return result;
    }

    // Property: friendlyname - Device friendly name
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_friendlyname(FriendlynameInfo& response) const
    {
        string name;
        uint32_t result = _deviceInfo->FriendlyName(name);
        response.Name = name;

        return result;
    }

    // Property: platformname - Device Platform name
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_platformname(FriendlynameInfo& response) const
    {
        string name;
        uint32_t result = _deviceInfo->PlatformName(name);
        response.Name = name;

        return result;
    }

    // Property: devicetype - Device type
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_devicetype(DevicetypeData& response) const
    {
        string deviceType;

        uint32_t result = _deviceInfo->DeviceType(deviceType);
        if (result == Core::ERROR_NONE) {
            Core::EnumerateType<JsonData::DeviceInfo::DevicetypeData::DevicetypeType> value(deviceType.c_str(), false);
            if (value.IsSet()) {
                response.Devicetype = value.Value();
            } else {
                TRACE(Trace::Fatal, (_T("Unknown value %s"), deviceType.c_str()));
                result = Core::ERROR_GENERAL;
            }
        }

        return result;
    }

    // Property: distributorid - Partner ID or distributor ID for device
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_distributorid(DistributoridData& response) const
    {
        string distributorId;

        uint32_t result = _deviceInfo->DistributorId(distributorId);
        if (result == Core::ERROR_NONE) {
            Core::EnumerateType<JsonData::DeviceInfo::DistributoridData::DistributoridType> value(distributorId.c_str(), false);
            if (value.IsSet()) {
                response.Distributorid = value.Value();
            } else {
                TRACE(Trace::Fatal, (_T("Unknown value %s"), distributorId.c_str()));
                result = Core::ERROR_GENERAL;
            }
        }

        return result;
    }

} // namespace Plugin

}
