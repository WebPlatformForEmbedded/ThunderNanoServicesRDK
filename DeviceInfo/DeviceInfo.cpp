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

    namespace {

        static Metadata<DeviceInfo> metadata(
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

    static Core::ProxyPoolType<Web::JSONBodyType<DeviceInfo::Data>> jsonResponseFactory(4);

    /* virtual */ const string DeviceInfo::Initialize(PluginHost::IShell* service)
    {
        ASSERT(_service == nullptr);
        ASSERT(_subSystem == nullptr);
        ASSERT(_deviceInfo == nullptr);
        ASSERT(service != nullptr);
        ASSERT(_connectionId == 0);

        string message;
        Config config;
        config.FromString(service->ConfigLine());
        _skipURL = static_cast<uint8_t>(service->WebPrefix().length());
        _service = service;
        _service->AddRef();
        _service->Register(&_notification);
        _subSystem = service->SubSystems();

        if (_subSystem == nullptr) {
            message = _T("DeviceInfo Susbsystem could not be obtained");
        } else {
            _subSystem->AddRef();
            _subSystem->Register(&_notification);

            _deviceInfo = _service->Root<Exchange::IDeviceInfo>(_connectionId, 2000, _T("DeviceInfoImplementation"));
            if (_deviceInfo == nullptr) {
                message = _T("DeviceInfo could not be instantiated");
                SYSLOG(Logging::Startup, (_T("DeviceInfo could not be instantiated")));
            } else {
                _deviceInfo->Configure(_service);
                _deviceAudioCapabilityInterface = _deviceInfo->QueryInterface<Exchange::IDeviceAudioCapabilities>();
                if (_deviceAudioCapabilityInterface == nullptr) {
                    message = _T("DeviceInfo Audio Capabilities Interface could not be instantiated");
                } else {
                    _deviceVideoCapabilityInterface = _deviceInfo->QueryInterface<Exchange::IDeviceVideoCapabilities>();
                    if (_deviceVideoCapabilityInterface == nullptr) {
                        message = _T("DeviceInfo Video Capabilities Interface could not be instantiated");
                    } else {
                        RegisterAll();
                    }
                }
            }
        }

        // On success return empty, to indicate there is no error text.
        return message;
    }

    /* virtual */ void DeviceInfo::Deinitialize(PluginHost::IShell* service VARIABLE_IS_NOT_USED)
    {
        if (_service != nullptr) {
            ASSERT(_service == service);

            _service->Unregister(&_notification);

            if (_subSystem != nullptr) {
                _subSystem->Unregister(&_notification);
                _subSystem->Release();
                _subSystem = nullptr;
            }

            if (_deviceInfo != nullptr){

                if (_deviceAudioCapabilityInterface != nullptr) {
                    _deviceAudioCapabilityInterface->Release();
                    _deviceAudioCapabilityInterface = nullptr;
                }
                if (_deviceVideoCapabilityInterface != nullptr) {
                    UnregisterAll();
                    _deviceVideoCapabilityInterface->Release();
                    _deviceVideoCapabilityInterface = nullptr;
                }

                RPC::IRemoteConnection* connection(_service->RemoteConnection(_connectionId));
                VARIABLE_IS_NOT_USED uint32_t result = _deviceInfo->Release();
                _deviceInfo = nullptr;
                // It should have been the last reference we are releasing,
                // so it should endup in a DESTRUCTION_SUCCEEDED, if not we
                // are leaking...
                ASSERT(result == Core::ERROR_DESTRUCTION_SUCCEEDED);
                // The process can disappear in the meantime...
                if (connection != nullptr) {
                    // But if it did not dissapear in the meantime, forcefully terminate it. Shoot to kill :-)
                    connection->Terminate();
                    connection->Release();
                }
            }

            _service->Release();
            _service = nullptr;
            _connectionId = 0;
        }
    }

    /* virtual */ string DeviceInfo::Information() const
    {
        // No additional info to report.
        return (string());
    }

    /* virtual */ void DeviceInfo::Inbound(Web::Request& /* request */)
    {
    }

    /* virtual */ Core::ProxyType<Web::Response> DeviceInfo::Process(const Web::Request& request)
    {
        ASSERT(_skipURL <= request.Path.length());

        Core::ProxyType<Web::Response> result(PluginHost::IFactories::Instance().Response());

        // By default, we assume everything works..
        result->ErrorCode = Web::STATUS_OK;
        result->Message = "OK";

        // <GET> - currently, only the GET command is supported, returning system info
        if (request.Verb == Web::Request::HTTP_GET) {

            Core::ProxyType<Web::JSONBodyType<Data>> response(jsonResponseFactory.Element());

            Core::TextSegmentIterator index(Core::TextFragment(request.Path, _skipURL, static_cast<uint32_t>(request.Path.length()) - _skipURL), false, '/');

            // Always skip the first one, it is an empty part because we start with a '/' if there are more parameters.
            index.Next();

            if (index.Next() == false) {
                AddressInfo(response->Addresses);
                SysInfo(response->SystemInfo);
                SocketPortInfo(response->Sockets);
            } else if (index.Current() == "Adresses") {
                AddressInfo(response->Addresses);
            } else if (index.Current() == "System") {
                SysInfo(response->SystemInfo);
            } else if (index.Current() == "Sockets") {
                SocketPortInfo(response->Sockets);
            }
            // TODO RB: I guess we should do something here to return other info (e.g. time) as well.

            result->ContentType = Web::MIMETypes::MIME_JSON;
            result->Body(Core::ProxyType<Web::IBody>(response));
        } else {
            result->ErrorCode = Web::STATUS_BAD_REQUEST;
            result->Message = _T("Unsupported request for the [DeviceInfo] service.");
        }

        return result;
    }

    void DeviceInfo::SysInfo(JsonData::DeviceInfo::SysteminfoData& systemInfo) const
    {
        Core::SystemInfo& singleton(Core::SystemInfo::Instance());

        systemInfo.Time = Core::Time::Now().ToRFC1123(true);
        systemInfo.Version = _subSystem->Version() + _T("#") + _subSystem->BuildTreeHash();
        systemInfo.Uptime = singleton.GetUpTime();
        systemInfo.Freeram = singleton.GetFreeRam();
        systemInfo.Totalram = singleton.GetTotalRam();
        systemInfo.Devicename = singleton.GetHostName();
        systemInfo.Cpuload = Core::NumberType<uint32_t>(static_cast<uint32_t>(singleton.GetCpuLoad())).Text();

        _adminLock.Lock();
        systemInfo.Serialnumber = _deviceId;
        _adminLock.Unlock();
    }

    void DeviceInfo::AddressInfo(Core::JSON::ArrayType<JsonData::DeviceInfo::AddressesData>& addressInfo) const
    {
        // Get the point of entry on WPEFramework..
        Core::AdapterIterator interfaces;

        while (interfaces.Next() == true) {

            JsonData::DeviceInfo::AddressesData newElement;
            newElement.Name = interfaces.Name();
            newElement.Mac = interfaces.MACAddress(':');
            JsonData::DeviceInfo::AddressesData& element(addressInfo.Add(newElement));

            // get an interface with a public IP address, then we will have a proper MAC address..
            Core::IPV4AddressIterator selectedNode(interfaces.IPV4Addresses());

            while (selectedNode.Next() == true) {
                Core::JSON::String nodeName;
                nodeName = selectedNode.Address().HostAddress();

                element.Ip.Add(nodeName);
            }
        }
    }

    void DeviceInfo::SocketPortInfo(JsonData::DeviceInfo::SocketinfoData& socketPortInfo) const
    {
        socketPortInfo.Runs = Core::ResourceMonitor::Instance().Runs();
    }

    uint32_t DeviceInfo::AudioOutputs(AudioOutputTypes& audioOutputs) const
    {
        Exchange::IDeviceAudioCapabilities::IAudioOutputIterator* audioIt;

        uint32_t status = _deviceAudioCapabilityInterface->AudioOutputs(audioIt);
        if ((status == Core::ERROR_NONE) && (audioIt != nullptr)) {
            Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput;
            Core::JSON::EnumType<JsonData::DeviceInfo::AudioportType> jsonAudioOutput;
            while (audioIt->Next(audioOutput) == true) {
                jsonAudioOutput = static_cast<JsonData::DeviceInfo::AudioportType>(audioOutput);
                audioOutputs.Add(jsonAudioOutput);
            }
            audioIt->Release();
        }
        return status;
    }

    uint32_t DeviceInfo::AudioCapabilities(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, AudioCapabilityTypes& audioCapabilityTypes) const
    {
        Exchange::IDeviceAudioCapabilities::IAudioCapabilityIterator* audioCapabilityIt = nullptr;
        Exchange::IDeviceAudioCapabilities::AudioCapability audioCapabilty(
            Exchange::IDeviceAudioCapabilities::AudioCapability::AUDIOCAPABILITY_NONE);

        uint32_t status = _deviceAudioCapabilityInterface->AudioCapabilities(audioOutput, audioCapabilityIt);
        if ((status == Core::ERROR_NONE) && (audioCapabilityIt != nullptr)) {
            Core::JSON::EnumType<JsonData::DeviceInfo::AudiocapabilityType> jsonAudioCapability;
            while (audioCapabilityIt->Next(audioCapabilty)) {
                jsonAudioCapability = static_cast<JsonData::DeviceInfo::AudiocapabilityType>(audioCapabilty);
                audioCapabilityTypes.Add(jsonAudioCapability);
            }
            audioCapabilityIt->Release();
        }

        return (status);
    }

    uint32_t DeviceInfo::Ms12Capabilities(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Ms12CapabilityTypes& ms12CapabilityTypes) const
    {
        Exchange::IDeviceAudioCapabilities::IMS12CapabilityIterator* ms12CapabilityIt = nullptr;
        Exchange::IDeviceAudioCapabilities::MS12Capability ms12Capabilty(
            Exchange::IDeviceAudioCapabilities::MS12Capability::MS12CAPABILITY_NONE);

        uint32_t status = _deviceAudioCapabilityInterface->MS12Capabilities(audioOutput, ms12CapabilityIt);
        if ((status == Core::ERROR_NONE) && (ms12CapabilityIt != nullptr)) {
            Core::JSON::EnumType<JsonData::DeviceInfo::Ms12capabilityType> jsonMs12Capability;
            while (ms12CapabilityIt->Next(ms12Capabilty)) {
                jsonMs12Capability = static_cast<JsonData::DeviceInfo::Ms12capabilityType>(ms12Capabilty);
                ms12CapabilityTypes.Add(jsonMs12Capability);
            }
            ms12CapabilityIt->Release();
        }

        return (status);
    }

    uint32_t DeviceInfo::Ms12Profiles(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Ms12ProfileTypes& ms12ProfileTypes) const
    {
        Exchange::IDeviceAudioCapabilities::IMS12ProfileIterator* ms12ProfileIt = nullptr;
        Exchange::IDeviceAudioCapabilities::MS12Profile ms12Profile(
            Exchange::IDeviceAudioCapabilities::MS12Profile::MS12PROFILE_NONE);

        uint32_t status = _deviceAudioCapabilityInterface->MS12AudioProfiles(audioOutput, ms12ProfileIt);
        if ((status == Core::ERROR_NONE) && (ms12ProfileIt != nullptr)) {
            Core::JSON::EnumType<JsonData::DeviceInfo::Ms12profileType> jsonMs12Profile;
            while (ms12ProfileIt->Next(ms12Profile)) {
                jsonMs12Profile = static_cast<JsonData::DeviceInfo::Ms12profileType>(ms12Profile);
                ms12ProfileTypes.Add(jsonMs12Profile);
            }
            ms12ProfileIt->Release();
        }

        return (status);
    }

    void DeviceInfo::AudioCapabilitiesInfo(JsonData::DeviceInfo::DeviceaudiocapabilitiesData& response) const
    {
        Exchange::IDeviceAudioCapabilities::IAudioOutputIterator* audioIt = nullptr;
        if ((_deviceAudioCapabilityInterface->AudioOutputs(audioIt) == Core::ERROR_NONE) &&
            (audioIt != nullptr)) {
            Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput;

            while (audioIt->Next(audioOutput)) {
                JsonData::DeviceInfo::DeviceaudiocapabilitiesData::AudiooutputcapabilitiesData audiocapabilities;
                audiocapabilities.AudioPort =
                    static_cast<WPEFramework::JsonData::DeviceInfo::AudioportType>(audioOutput);

                AudioCapabilities(audioOutput, audiocapabilities.Audiocapabilities);
                Ms12Capabilities(audioOutput, audiocapabilities.Ms12capabilities);
                Ms12Profiles(audioOutput, audiocapabilities.Ms12profiles);

                response.Audiooutputcapabilities.Add(audiocapabilities);
            }
            audioIt->Release();
        }
    }

    uint32_t DeviceInfo::VideoOutputs(VideoOutputTypes& videoOutputs) const
    {
        Exchange::IDeviceVideoCapabilities::IVideoOutputIterator* videoIt;

        uint32_t status = _deviceVideoCapabilityInterface->VideoOutputs(videoIt);
        if ((status == Core::ERROR_NONE) && (videoIt != nullptr)) {
            Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput;
            Core::JSON::EnumType<JsonData::DeviceInfo::VideodisplayType> jsonVideoOutput;
            while (videoIt->Next(videoOutput) == true) {
                jsonVideoOutput = static_cast<JsonData::DeviceInfo::VideodisplayType>(videoOutput);
                videoOutputs.Add(jsonVideoOutput);
            }
            videoIt->Release();
        }
        return status;
    }

    uint32_t DeviceInfo::DefaultResolution(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, ScreenResolutionType& screenResolutionType) const
    {
        Exchange::IDeviceVideoCapabilities::ScreenResolution defaultResolution(
            Exchange::IDeviceVideoCapabilities::ScreenResolution::ScreenResolution_Unknown);
        uint32_t status = _deviceVideoCapabilityInterface->DefaultResolution(videoOutput, defaultResolution);

        if (status == Core::ERROR_NONE) {
            screenResolutionType = static_cast<JsonData::DeviceInfo::Output_resolutionType>(defaultResolution);
        }
        return (status);
    }

    uint32_t DeviceInfo::Resolutions(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, ScreenResolutionTypes& screenResolutionTypes) const
    {
        Exchange::IDeviceVideoCapabilities::IScreenResolutionIterator* resolutionIt = nullptr;
        Exchange::IDeviceVideoCapabilities::ScreenResolution resolution(
            Exchange::IDeviceVideoCapabilities::ScreenResolution::ScreenResolution_Unknown);

        uint32_t status = _deviceVideoCapabilityInterface->Resolutions(videoOutput, resolutionIt);
        if ((status == Core::ERROR_NONE) && (resolutionIt != nullptr)) {
            Core::JSON::EnumType<JsonData::DeviceInfo::Output_resolutionType> jsonResolution;
            while (resolutionIt->Next(resolution)) {
                jsonResolution = static_cast<JsonData::DeviceInfo::Output_resolutionType>(resolution);
                screenResolutionTypes.Add(jsonResolution);
            }
            resolutionIt->Release();
        }
        return (status);
    }

    uint32_t DeviceInfo::Hdcp(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, CopyProtectionType& copyProtectionType) const
    {
        Exchange::IDeviceVideoCapabilities::CopyProtection hdcp(
            Exchange::IDeviceVideoCapabilities::CopyProtection::HDCP_UNAVAILABLE);

        uint32_t status = _deviceVideoCapabilityInterface->Hdcp(videoOutput, hdcp);
        if (status == Core::ERROR_NONE) {
            copyProtectionType = static_cast<JsonData::DeviceInfo::CopyprotectionType>(hdcp);
        }
        return (status);
    }

    void DeviceInfo::VideoCapabilitiesInfo(JsonData::DeviceInfo::DevicevideocapabilitiesData& response) const
    {
        ASSERT(_deviceVideoCapabilityInterface != nullptr);

        bool supportsHdr = false;
        if (_deviceVideoCapabilityInterface->HDR(supportsHdr) == Core::ERROR_NONE) {
            response.Hdr = supportsHdr;
        }

        bool supportsAtmos = false;
        if (_deviceVideoCapabilityInterface->Atmos(supportsAtmos) == Core::ERROR_NONE) {
            response.Atmos = supportsAtmos;
        }

        bool supportsCec = false;
        if (_deviceVideoCapabilityInterface->CEC(supportsCec) == Core::ERROR_NONE) {
            response.Cec = supportsCec;
        }

        HostEDID(response.Hostedid);

        Exchange::IDeviceVideoCapabilities::IVideoOutputIterator* videoIt;
        if ((_deviceVideoCapabilityInterface->VideoOutputs(videoIt) == Core::ERROR_NONE) &&
            (videoIt != nullptr)) {
            Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput;
            while (videoIt->Next(videoOutput) == true) {
                JsonData::DeviceInfo::DevicevideocapabilitiesData::VideooutputcapabilitiesData videocapabilities;
                videocapabilities.VideoDisplay =
                    static_cast<WPEFramework::JsonData::DeviceInfo::VideodisplayType>(videoOutput);

                Hdcp(videoOutput, videocapabilities.Hdcp);

                DefaultResolution(videoOutput, videocapabilities.Defaultresolution);

                Resolutions(videoOutput, videocapabilities.Output_resolutions);

                response.Videooutputcapabilities.Add(videocapabilities);
            }
            videoIt->Release();
        }
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

    void DeviceInfo::UpdateDeviceIdentifier()
    {
        ASSERT(_subSystem != nullptr);

        if ((_deviceId.empty() == true) && (_subSystem->IsActive(PluginHost::ISubSystem::IDENTIFIER) == true)) {

            const PluginHost::ISubSystem::IIdentifier* identifier(_subSystem->Get<PluginHost::ISubSystem::IIdentifier>());

            if (identifier != nullptr) {
                uint8_t buffer[64];

                if ((buffer[0] = identifier->Identifier(sizeof(buffer) - 1, &(buffer[1]))) != 0) {
                    _adminLock.Lock();
                    _deviceId = Core::SystemInfo::Instance().Id(buffer, ~0);
                    _adminLock.Unlock();
                }
                identifier->Release();
            }
        }
    }

    void DeviceInfo::Deactivated(RPC::IRemoteConnection* connection)
    {
        if (_connectionId == connection->Id()) {

            ASSERT(_service != nullptr);
            Core::IWorkerPool::Instance().Submit(PluginHost::IShell::Job::Create(_service, PluginHost::IShell::DEACTIVATED, PluginHost::IShell::FAILURE));
        }
    }
} // namespace Plugin
} // namespace WPEFramework
