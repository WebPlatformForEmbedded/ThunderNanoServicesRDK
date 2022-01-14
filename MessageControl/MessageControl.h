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

#pragma once

#include "Module.h"

namespace WPEFramework {
namespace Plugin {

    class MessageControl : public PluginHost::IPlugin, public PluginHost::JSONRPC {
    private:
        class Observer : public RPC::IRemoteConnection::INotification {
        public:
            Observer(MessageControl& parent);

            void Activated(RPC::IRemoteConnection* connection) override;
            void Deactivated(RPC::IRemoteConnection* connection) override;

        private:
            friend class Core::ThreadPool::JobType<Observer&>;

            void Dispatch();

            BEGIN_INTERFACE_MAP(Observer)
            INTERFACE_ENTRY(RPC::IRemoteConnection::INotification)
            END_INTERFACE_MAP

            Core::CriticalSection _adminLock;
            MessageControl& _parent;

            std::list<uint32_t> _activationIds;
            std::list<uint32_t> _deactivationIds;
            Core::WorkerPool::JobType<Observer&> _job;
        };

    public:
        MessageControl(const MessageControl&) = delete;
        MessageControl& operator=(const MessageControl&) = delete;

        MessageControl();
        ~MessageControl() override;

        BEGIN_INTERFACE_MAP(MessageControl)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IDispatcher)
        INTERFACE_AGGREGATE(Exchange::IMessageControl, _control)
        END_INTERFACE_MAP

    public:
        const string Initialize(PluginHost::IShell* service) override;
        void Deinitialize(PluginHost::IShell* service) override;
        string Information() const override;

        //JSONRPC
        void RegisterAll();
        void UnregisterAll();
        uint32_t endpoint_set(const JsonData::MessageControl::MessageInfo& params);
        uint32_t endpoint_status(const JsonData::MessageControl::StatusParamsData& params, JsonData::MessageControl::StatusResultData& response);

    private:
        void Activated(const uint32_t id);
        void Deactivated(const uint32_t id);

        uint32_t _connectionId;
        Exchange::IMessageControl* _control;
        Core::Sink<Observer> _observer;
    };

} // namespace Plugin
} // namespace WPEFramework
