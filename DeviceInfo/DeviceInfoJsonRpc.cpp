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
        Register<ResolutionsParamsInfo,ResolutionsResultData>(_T("resolutions"), &DeviceInfo::endpoint_resolutions, this);
        Register<ResolutionsParamsInfo,DefaultresolutionResultData>(_T("defaultresolution"), &DeviceInfo::endpoint_defaultresolution, this);
        Register<ResolutionsParamsInfo,HdcpResultData>(_T("hdcp"), &DeviceInfo::endpoint_hdcp, this);
        Register<AudiocapabilitiesParamsInfo,AudiocapabilitiesResultData>(_T("audiocapabilities"), &DeviceInfo::endpoint_audiocapabilities, this);
        Register<AudiocapabilitiesParamsInfo,Ms12capabilitiesResultData>(_T("ms12capabilities"), &DeviceInfo::endpoint_ms12capabilities, this);
        Register<AudiocapabilitiesParamsInfo,Ms12audioprofilesResultData>(_T("ms12audioprofiles"), &DeviceInfo::endpoint_ms12audioprofiles, this);
        Property<DeviceaudiocapabilitiesData>(_T("deviceaudiocapabilities"), &DeviceInfo::endpoint_get_deviceaudiocapabilities, nullptr, this);
        Property<DevicevideocapabilitiesData>(_T("devicevideocapabilities"), &DeviceInfo::endpoint_get_devicevideocapabilities, nullptr, this);
        Property<DeviceinfoData>(_T("deviceinfo"), &DeviceInfo::endpoint_get_deviceinfo, nullptr, this);
        Property<SysteminfoData>(_T("systeminfo"), &DeviceInfo::endpoint_get_systeminfo, nullptr, this);
        Property<Core::JSON::ArrayType<AddressesData>>(_T("addresses"), &DeviceInfo::endpoint_get_addresses, nullptr, this);
        Property<SocketinfoData>(_T("socketinfo"), &DeviceInfo::endpoint_get_socketinfo, nullptr, this);
        Property<AudiooutputsData>(_T("audiooutputs"), &DeviceInfo::endpoint_get_audiooutputs, nullptr, this);
        Property<VideooutputsData>(_T("videooutputs"), &DeviceInfo::endpoint_get_videooutputs, nullptr, this);
        Property<HostedidData>(_T("hostedid"), &DeviceInfo::endpoint_get_hostedid, nullptr, this);
    }

    void DeviceInfo::UnregisterAll()
    {
        Unregister(_T("ms12audioprofiles"));
        Unregister(_T("ms12capabilities"));
        Unregister(_T("audiocapabilities"));
        Unregister(_T("hdcp"));
        Unregister(_T("defaultresolution"));
        Unregister(_T("resolutions"));
        Unregister(_T("hostedid"));
        Unregister(_T("videooutputs"));
        Unregister(_T("audiooutputs"));
        Unregister(_T("socketinfo"));
        Unregister(_T("addresses"));
        Unregister(_T("systeminfo"));
        Unregister(_T("deviceinfo"));
        Unregister(_T("devicevideocapabilities"));
        Unregister(_T("deviceaudiocapabilities"));
    }

    // API implementation
    //

    // Method: resolutions - Supported resolutions on the selected video display port
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_resolutions(const ResolutionsParamsInfo& params, ResolutionsResultData& response)
    {
        const VideooutputType& videooutput = params.Videooutput.Value();
        return Resolutions(static_cast<Exchange::IDeviceVideoCapabilities::VideoOutput>(videooutput), response.Resolutions);
    }

    // Method: defaultresolution - Default resolution on the selected video display port
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_defaultresolution(const ResolutionsParamsInfo& params, DefaultresolutionResultData& response)
    {
        const VideooutputType& videooutput = params.Videooutput.Value();
        return DefaultResolution(static_cast<Exchange::IDeviceVideoCapabilities::VideoOutput>(videooutput), response.Defaultresolution);
    }

    // Method: hdcp - Supported hdcp version on the selected video display port
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_hdcp(const ResolutionsParamsInfo& params, HdcpResultData& response)
    {
        const VideooutputType& videooutput = params.Videooutput.Value();
        return Hdcp(static_cast<Exchange::IDeviceVideoCapabilities::VideoOutput>(videooutput), response.Hdcpversion);
    }

    // Method: audiocapabilities - Audio capabilities for the specified audio port
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_audiocapabilities(const AudiocapabilitiesParamsInfo& params, AudiocapabilitiesResultData& response)
    {
        const AudiooutputType& audiooutput = params.Audiooutput.Value();
        return AudioCapabilities(static_cast<Exchange::IDeviceAudioCapabilities::AudioOutput>(audiooutput), response.Audiocapabilities);
    }

    // Method: ms12capabilities - Audio ms12 capabilities for the specified audio port
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_ms12capabilities(const AudiocapabilitiesParamsInfo& params, Ms12capabilitiesResultData& response)
    {
        const AudiooutputType& audiooutput = params.Audiooutput.Value();
        return Ms12Capabilities(static_cast<Exchange::IDeviceAudioCapabilities::AudioOutput>(audiooutput), response.Ms12capabilities);
    }

    // Method: ms12audioprofiles - Supported ms12 audio profiles for the specified audio port
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_ms12audioprofiles(const AudiocapabilitiesParamsInfo& params, Ms12audioprofilesResultData& response)
    {
        const AudiooutputType& audiooutput = params.Audiooutput.Value();
        return Ms12Profiles(static_cast<Exchange::IDeviceAudioCapabilities::AudioOutput>(audiooutput), response.Ms12profiles);
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

    // Property: audiooutputs - Audio ports supported on the device (all ports that are physically present)
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_audiooutputs(AudiooutputsData& response) const
    {
        return AudioOutputs(response.Audiooutputs);
    }

    // Property: videooutputs - Video ports supported on the device (all ports that are physically present)
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_videooutputs(VideooutputsData& response) const
    {
        return VideoOutputs(response.Videooutputs);
    }

    // Property: hostedid - EDID of the host
    // Return codes:
    //  - ERROR_NONE: Success
    //  - ERROR_GENERAL:
    uint32_t DeviceInfo::endpoint_get_hostedid(HostedidData& response) const
    {
        return HostEDID(response.Edid);
    }

} // namespace Plugin

}

