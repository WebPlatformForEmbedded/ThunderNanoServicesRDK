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

#include "DeviceInfo.h"

namespace WPEFramework {
namespace Plugin {

    uint32_t DeviceInfo::AudioOutputs(AudioOutputTypes& audioOutputs) const
    {
        return status;
    }

    uint32_t DeviceInfo::AudioCapabilities(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, AudioCapabilityTypes& audioCapabilityTypes) const
    {

        return (status);
    }

    uint32_t DeviceInfo::Ms12Capabilities(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Ms12CapabilityTypes& ms12CapabilityTypes) const
    {
        return (status);
    }

    uint32_t DeviceInfo::Ms12Profiles(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Ms12ProfileTypes& ms12ProfileTypes) const
    {
        return (status);
    }

    void DeviceInfo::AudioCapabilitiesInfo(JsonData::DeviceInfo::DeviceaudiocapabilitiesData& response) const
    {
    }

    uint32_t DeviceInfo::VideoOutputs(VideoOutputTypes& videoOutputs) const
    {
        return status;
    }

    uint32_t DeviceInfo::DefaultResolution(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, ScreenResolutionType& screenResolutionType) const
    {
        return (status);
    }

    uint32_t DeviceInfo::Resolutions(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, ScreenResolutionTypes& screenResolutionTypes) const
    {
        return (status);
    }

    uint32_t DeviceInfo::Hdcp(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, CopyProtectionType& copyProtectionType) const
    {
        return (status);
    }

    void DeviceInfo::VideoCapabilitiesInfo(JsonData::DeviceInfo::DevicevideocapabilitiesData& response) const
    {
    }

    void DeviceInfo::DeviceMetaData(JsonData::DeviceInfo::DeviceinfoData& response) const
    {
        ASSERT(_deviceInfo != nullptr);
        string localresult ;

        if (_deviceInfo->DeviceType(localresult) == Core::ERROR_NONE) {
            response.Devicetype = localresult;
        }

        if (_deviceInfo->DistributorId(localresult) == Core::ERROR_NONE) {
            response.Distributorid = localresult;
        }

        if (_deviceInfo->FriendlyName(localresult) == Core::ERROR_NONE) {
            response.Friendlyname = localresult;
        }

        if (_deviceInfo->Make(localresult) == Core::ERROR_NONE) {
            response.Make = localresult;
        }

        if (_deviceInfo->ModelName(localresult) == Core::ERROR_NONE) {
            response.Modelname = localresult;
        }

        uint16_t year = 0;
        if (_deviceInfo->ModelYear(year) == Core::ERROR_NONE) {
            response.Modelyear = year;
        }

        if (_deviceInfo->PlatformName(localresult) == Core::ERROR_NONE) {
            response.Platformname = localresult;
        }

        if (_deviceInfo->SerialNumber(localresult) == Core::ERROR_NONE) {
            response.Serialnumber = localresult;
        }

        if (_deviceInfo->Sku(localresult) == Core::ERROR_NONE) {
            response.Sku = localresult;
        }
    }

    uint32_t DeviceInfo::FirmwareVersion(JsonData::DeviceInfo::FirmwareversionData& response VARIABLE_IS_NOT_USED) const
    {
        uint32_t result = Core::ERROR_GENERAL;

        // imagename is required
        string value;
        if (_deviceFirmwareVersionInterface->Imagename(value) == Core::ERROR_NONE) {
            response.Imagename = value;
            result = Core::ERROR_NONE;

            if (_deviceFirmwareVersionInterface->Sdk(value) == Core::ERROR_NONE)
                response.Sdk = value;
            if (_deviceFirmwareVersionInterface->Mediarite(value) == Core::ERROR_NONE)
                response.Mediarite = value;

            if (_deviceFirmwareVersionInterface->Yocto(value) == Core::ERROR_NONE) {
                Core::EnumerateType<FirmwareversionData::YoctoType> yocto(value.c_str(), false);
                if (yocto.IsSet()) {
                    response.Yocto = yocto.Value();
                } else {
                    TRACE(Trace::Fatal, (_T("Unknown value %s"), value.c_str()));
                    result = Core::ERROR_GENERAL;
                }
            }
        }
        return result;
    }

} // namespace Plugin
} // namespace WPEFramework
