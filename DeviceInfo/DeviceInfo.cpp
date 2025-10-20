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

namespace Thunder {
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

    /* virtual */ const string DeviceInfo::Initialize(PluginHost::IShell* service)
    {
        ASSERT(_service == nullptr);
        ASSERT(_subSystem == nullptr);
        ASSERT(_deviceInfo == nullptr);
        ASSERT(_configuration == nullptr);
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
                _configuration = _deviceInfo->QueryInterface<Exchange::IConfiguration>();
                if (_configuration == nullptr) {
                    message = _T("DeviceInfo Configuration Interface could not be instantiated");
                } else {
                    _configuration->Configure(_service);
                    _deviceAudioCapabilityInterface = _deviceInfo->QueryInterface<Exchange::IDeviceAudioCapabilities>();
                    if (_deviceAudioCapabilityInterface == nullptr) {
                        message = _T("DeviceInfo Audio Capabilities Interface could not be instantiated");
                    } else {
                        _deviceVideoCapabilityInterface = _deviceInfo->QueryInterface<Exchange::IDeviceVideoCapabilities>();
                        if (_deviceVideoCapabilityInterface == nullptr) {
                            message = _T("DeviceInfo Video Capabilities Interface could not be instantiated");
                        } else {
                            Exchange::JSystemInfo::Register(*this, this);
                            Exchange::JDeviceInfo::Register(*this, _deviceInfo);
                            Exchange::JDeviceAudioCapabilities::Register(*this, _deviceAudioCapabilityInterface);
                            Exchange::JDeviceVideoCapabilities::Register(*this, _deviceVideoCapabilityInterface);
                        }
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
                    Exchange::JDeviceAudioCapabilities::Unregister(*this);
                    _deviceAudioCapabilityInterface->Release();
                    _deviceAudioCapabilityInterface = nullptr;
                }
                if (_deviceVideoCapabilityInterface != nullptr) {
                    Exchange::JDeviceVideoCapabilities::Unregister(*this);
                    _deviceVideoCapabilityInterface->Release();
                    _deviceVideoCapabilityInterface = nullptr;
                }
                if (_configuration != nullptr) {
                    _configuration->Release();
                    _configuration = nullptr;
                }

                RPC::IRemoteConnection* connection(_service->RemoteConnection(_connectionId));
                Exchange::JDeviceInfo::Unregister(*this);
                VARIABLE_IS_NOT_USED uint32_t result = _deviceInfo->Release();
                _deviceInfo = nullptr;
                Exchange::JSystemInfo::Unregister(*this);
                // It should have been the last reference we are releasing,
                // so it should endup in a DESTRUCTION_SUCCEEDED, if not we
                // are leaking...
                ASSERT( (result == Core::ERROR_CONNECTION_CLOSED) || (result == Core::ERROR_DESTRUCTION_SUCCEEDED));
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

    Core::hresult DeviceInfo::AudioOutputs(Exchange::IDeviceAudioCapabilities::IAudioOutputIterator*& audioOutputs) const
    {
        Core::SafeSyncType<Core::CriticalSection> lock(_adminLock);
        return (_deviceAudioCapabilityInterface->AudioOutputs(audioOutputs));
    }

    Core::hresult DeviceInfo::AudioCapabilities(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Exchange::IDeviceAudioCapabilities::IAudioCapabilityIterator*& audioCapabilities) const
    {
        Core::SafeSyncType<Core::CriticalSection> lock(_adminLock);
        return (_deviceAudioCapabilityInterface->AudioCapabilities(audioOutput, audioCapabilities));
    }

    Core::hresult DeviceInfo::MS12Capabilities(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Exchange::IDeviceAudioCapabilities::IMS12CapabilityIterator*& ms12Capabilities) const
    {
        Core::SafeSyncType<Core::CriticalSection> lock(_adminLock);
        return (_deviceAudioCapabilityInterface->MS12Capabilities(audioOutput, ms12Capabilities));
    }

    Core::hresult DeviceInfo::MS12AudioProfiles(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Exchange::IDeviceAudioCapabilities::IMS12ProfileIterator*& ms12Profiles) const
    {
        Core::SafeSyncType<Core::CriticalSection> lock(_adminLock);
        return (_deviceAudioCapabilityInterface->MS12AudioProfiles(audioOutput, ms12Profiles));
    }

    Core::hresult DeviceInfo::VideoOutputs(Exchange::IDeviceVideoCapabilities::IVideoOutputIterator*& videoOutputs) const
    {
        Core::SafeSyncType<Core::CriticalSection> lock(_adminLock);
        return (_deviceVideoCapabilityInterface->VideoOutputs(videoOutputs));
    }

    Core::hresult DeviceInfo::DefaultResolution(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, Exchange::IDeviceVideoCapabilities::ScreenResolution& defaultResolution) const
    {
        Core::SafeSyncType<Core::CriticalSection> lock(_adminLock);
        return (_deviceVideoCapabilityInterface->DefaultResolution(videoOutput, defaultResolution));
    }

    Core::hresult DeviceInfo::Resolutions(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, Exchange::IDeviceVideoCapabilities::IScreenResolutionIterator*& resolutions) const
    {
        Core::SafeSyncType<Core::CriticalSection> lock(_adminLock);
        return (_deviceVideoCapabilityInterface->Resolutions(videoOutput, resolutions));
    }

    Core::hresult DeviceInfo::Hdcp(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, Exchange::IDeviceVideoCapabilities::CopyProtection& hdcpVersion) const
    {
        Core::SafeSyncType<Core::CriticalSection> lock(_adminLock);
        return (_deviceVideoCapabilityInterface->Hdcp(videoOutput, hdcpVersion));
    }

    Core::hresult DeviceInfo::HostEDID(string& edid) const
    {
        Core::SafeSyncType<Core::CriticalSection> lock(_adminLock);
        return (_deviceVideoCapabilityInterface->HostEDID(edid));
    }

    Core::hresult DeviceInfo::Version(string& value) const
    {
        _adminLock.Lock();
        value = (_subSystem->Version() + _T("#") + _subSystem->BuildTreeHash());
        _adminLock.Unlock();

        return (Core::ERROR_NONE);
    }

    Core::hresult DeviceInfo::Uptime(uint32_t& value) const
    {
        value = Core::SystemInfo::Instance().GetUpTime();

        return (Core::ERROR_NONE);
    }

    Core::hresult DeviceInfo::TotalRAM(uint64_t& value) const
    {
        value = Core::SystemInfo::Instance().GetTotalRam();

        return (Core::ERROR_NONE);
    }

    Core::hresult DeviceInfo::FreeRAM(uint64_t& value) const
    {
        value = Core::SystemInfo::Instance().GetFreeRam();

        return (Core::ERROR_NONE);
    }

    Core::hresult DeviceInfo::TotalSwap(uint64_t& value) const
    {
        value = Core::SystemInfo::Instance().GetTotalSwap();

        return (Core::ERROR_NONE);
    }

    Core::hresult DeviceInfo::FreeSwap(uint64_t& value) const
    {
        value = Core::SystemInfo::Instance().GetFreeSwap();

        return (Core::ERROR_NONE);
    }

    Core::hresult DeviceInfo::DeviceName(string& value) const
    {
        value = Core::SystemInfo::Instance().GetHostName();

        return (Core::ERROR_NONE);
    }

    Core::hresult DeviceInfo::CPULoad(uint8_t& value) const
    {
        value = static_cast<uint8_t>(Core::SystemInfo::Instance().GetCpuLoad());

        return (Core::ERROR_NONE);
    }

    Core::hresult DeviceInfo::CPULoadAvgs(Exchange::ISystemInfo::CPULoads& value) const
    {
        uint64_t* loadAvgs = Core::SystemInfo::Instance().GetCpuLoadAvg();
        value.avg1min = static_cast<uint8_t>(loadAvgs[0]);
        value.avg5min = static_cast<uint8_t>(loadAvgs[1]);
        value.avg15min = static_cast<uint8_t>(loadAvgs[2]);

        return (Core::ERROR_NONE);
    }

    Core::hresult DeviceInfo::Addresses(Exchange::ISystemInfo::INetworkInterfaceIterator*& networkInterfaces) const
    {
        // Get the point of entry on Thunder..
        Core::AdapterIterator interfaces;
        std::vector<Exchange::ISystemInfo::NetworkInterface> list;

        while (interfaces.Next() == true) {

            Exchange::ISystemInfo::NetworkInterface newElement;

            newElement.name = interfaces.Name();
            newElement.mac = interfaces.MACAddress(':');

            // get an interface with a public IP address, then we will have a proper MAC address..
            Core::IPV4AddressIterator selectedNode(interfaces.IPV4Addresses());

            std::vector<string> ipAddresses;

            while (selectedNode.Next() == true) {
                ipAddresses.push_back(selectedNode.Address().HostAddress());
            }

            if (ipAddresses.size() > 0) {
                newElement.ipAddresses = std::move(ipAddresses);
            }

            list.push_back(newElement);
        }

        using Implementation = RPC::IteratorType<Exchange::ISystemInfo::INetworkInterfaceIterator>;
        networkInterfaces = Core::ServiceType<Implementation>::Create<Exchange::ISystemInfo::INetworkInterfaceIterator>(list);

        return (Core::ERROR_NONE);
    }

    Core::hresult DeviceInfo::SocketInfo(Exchange::ISystemInfo::Sockets& socketInfo) const
    {
        socketInfo.runs = Core::ResourceMonitor::Instance().Runs();

        return (Core::ERROR_NONE);
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
} // namespace Thunder
