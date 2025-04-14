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

#ifndef DEVICEINFO_DEVICEINFO_H
#define DEVICEINFO_DEVICEINFO_H

#include "Module.h"
#include <interfaces/IConfiguration.h>
#include <interfaces/IDeviceInfo.h>
#include <interfaces/json/JDeviceInfo.h>
#include <interfaces/json/JSystemInfo.h>
#include <interfaces/json/JDeviceAudioCapabilities.h>
#include <interfaces/json/JDeviceVideoCapabilities.h>

namespace Thunder {
namespace Plugin {

    class DeviceInfo : public PluginHost::IPlugin
                     , public PluginHost::JSONRPC
                     , public Exchange::ISystemInfo {
    private:
        class Notification : public PluginHost::ISubSystem::INotification, public RPC::IRemoteConnection::INotification {
        private:
            class Job {
            public:
                Job() = delete;
                Job(const Job&) = delete;
                Job& operator=(const Job&) = delete;

                Job(DeviceInfo& parent) : _parent(parent) { }
                ~Job() = default;

            public:
                void Dispatch() {
                    _parent.UpdateDeviceIdentifier();
                }

            private:
                DeviceInfo& _parent;
            };

        public:
            Notification() = delete;
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;

            explicit Notification(DeviceInfo& parent)
                : _parent(parent)
                , _job(parent) {
            }
            ~Notification() override {
                _job.Revoke();
            }

        public:
            // Some changes happened in the subsystems
            void Updated() override {
                _job.Submit();
            }
            void Activated(RPC::IRemoteConnection* /* connection */) override {
            }
            void Deactivated(RPC::IRemoteConnection* connection) override {
                _parent.Deactivated(connection);
            }
            void Terminated(RPC::IRemoteConnection* /* connection */) override {
            }

            BEGIN_INTERFACE_MAP(Notification)
                INTERFACE_ENTRY(PluginHost::ISubSystem::INotification)
                INTERFACE_ENTRY(RPC::IRemoteConnection::INotification)
            END_INTERFACE_MAP

        private:
            DeviceInfo& _parent;
            Core::WorkerPool::JobType<Job> _job;
        };

    public:
        DeviceInfo(const DeviceInfo&) = delete;
        DeviceInfo& operator=(const DeviceInfo&) = delete;
        DeviceInfo()
            : _skipURL(0)
            , _service(nullptr)
            , _subSystem(nullptr)
            , _deviceId()
            , _deviceInfo(nullptr)
            , _configuration(nullptr)
            , _deviceAudioCapabilityInterface(nullptr)
            , _deviceVideoCapabilityInterface(nullptr)
            , _connectionId(0)
            , _adminLock()
            , _notification(*this)
        {
        }

        ~DeviceInfo() override = default;

        BEGIN_INTERFACE_MAP(DeviceInfo)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IDispatcher)
        INTERFACE_ENTRY(Exchange::ISystemInfo)
        INTERFACE_AGGREGATE(Exchange::IDeviceInfo, _deviceInfo)
        INTERFACE_AGGREGATE(Exchange::IConfiguration, _configuration)
        INTERFACE_AGGREGATE(Exchange::IDeviceAudioCapabilities, _deviceAudioCapabilityInterface)
        INTERFACE_AGGREGATE(Exchange::IDeviceVideoCapabilities, _deviceVideoCapabilityInterface)
        END_INTERFACE_MAP

    public:
        //   IPlugin methods
        // -------------------------------------------------------------------------------------------------------
        virtual const string Initialize(PluginHost::IShell* service) override;
        virtual void Deinitialize(PluginHost::IShell* service) override;
        virtual string Information() const override;

    private:
        Core::hresult AudioOutputs(Exchange::IDeviceAudioCapabilities::IAudioOutputIterator*& audioOutputs) const;
        Core::hresult AudioCapabilities(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Exchange::IDeviceAudioCapabilities::IAudioCapabilityIterator*& audioCapabilities) const;
        Core::hresult MS12Capabilities(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Exchange::IDeviceAudioCapabilities::IMS12CapabilityIterator*& ms12Capabilities) const;
        Core::hresult MS12AudioProfiles(const Exchange::IDeviceAudioCapabilities::AudioOutput audioOutput, Exchange::IDeviceAudioCapabilities::IMS12ProfileIterator*& ms12Profiles) const;

        Core::hresult VideoOutputs(Exchange::IDeviceVideoCapabilities::IVideoOutputIterator*& videoOutputs) const;
        Core::hresult DefaultResolution(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, Exchange::IDeviceVideoCapabilities::ScreenResolution& defaultResolution) const;
        Core::hresult Resolutions(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, Exchange::IDeviceVideoCapabilities::IScreenResolutionIterator*& resolutions) const;
        Core::hresult Hdcp(const Exchange::IDeviceVideoCapabilities::VideoOutput videoOutput, Exchange::IDeviceVideoCapabilities::CopyProtection& hdcpVersion) const;
        Core::hresult HostEDID(string& edid) const;

        Core::hresult Version(string& value) const override;
        Core::hresult Uptime(uint32_t& value) const override;
        Core::hresult TotalRAM(uint64_t& value) const override;
        Core::hresult FreeRAM(uint64_t& value) const override;
        Core::hresult TotalSwap(uint64_t& value) const override;
        Core::hresult FreeSwap(uint64_t& value) const override;
        Core::hresult DeviceName(string& value) const override;
        Core::hresult CPULoad(uint8_t& value) const override;
        Core::hresult CPULoadAvgs(Exchange::ISystemInfo::CPULoads& value) const override;
        Core::hresult Addresses(Exchange::ISystemInfo::INetworkInterfaceIterator*& networkInterfaces) const override;
        Core::hresult SocketInfo(Exchange::ISystemInfo::Sockets& socketInfo) const override;

        void UpdateDeviceIdentifier();
        void Deactivated(RPC::IRemoteConnection* connection);

    private:
        uint8_t _skipURL;
        PluginHost::IShell* _service;
        PluginHost::ISubSystem* _subSystem;
        string _deviceId;
        Exchange::IDeviceInfo* _deviceInfo;
        Exchange::IConfiguration* _configuration;
        Exchange::IDeviceAudioCapabilities* _deviceAudioCapabilityInterface;
        Exchange::IDeviceVideoCapabilities* _deviceVideoCapabilityInterface;
        uint32_t _connectionId;
        mutable Core::CriticalSection _adminLock;
        Core::SinkType<Notification> _notification;
    };

} // namespace Plugin
} // namespace Thunder

#endif // DEVICEINFO_DEVICEINFO_H
