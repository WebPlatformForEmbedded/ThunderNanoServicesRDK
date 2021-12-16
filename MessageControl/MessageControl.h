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

    class MessageControl : public PluginHost::IPlugin {
    private:
        class Observer : public RPC::IRemoteConnection::INotification {
        public:
            Observer(MessageControl& parent)
                : _parent(parent)
            {
            }

            void Activated(RPC::IRemoteConnection* connection) override
            {
                _parent.Activated(connection);
            }
            void Deactivated(RPC::IRemoteConnection* connection) override
            {
                _parent.Deactivated(connection);
            }

        private:
            BEGIN_INTERFACE_MAP(Observer)
            INTERFACE_ENTRY(RPC::IRemoteConnection::INotification)
            END_INTERFACE_MAP

            void AddRef() const override
            {
                Core::InterlockedIncrement(_refcount);
            }
            uint32_t Release() const override
            {
                Core::InterlockedDecrement(_refcount);

                return Core::ERROR_NONE;
            }

            Core::CriticalSection _adminLock;
            MessageControl& _parent;
            mutable uint32_t _refcount;
        };

    public:
        MessageControl(const MessageControl&) = delete;
        MessageControl& operator=(const MessageControl&) = delete;

        MessageControl()
            : _connectionId(0)
            , _observer(*this)
        {
        }

        ~MessageControl() override = default;

        BEGIN_INTERFACE_MAP(MessageControl)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_AGGREGATE(Exchange::IMessageControl, _manager)
        END_INTERFACE_MAP

    public:
        virtual const string Initialize(PluginHost::IShell* service) override;
        virtual void Deinitialize(PluginHost::IShell* service) override;
        virtual string Information() const override;

    private:
        void Activated(RPC::IRemoteConnection* connection)
        {
            if (_manager != nullptr && connection != nullptr) {
                _manager->Activated(connection->Id());
            }
        }
        void Deactivated(RPC::IRemoteConnection* connection)
        {
            if (_manager != nullptr && connection != nullptr) {
                _manager->Deactivated(connection->Id());
            }
        }

        uint32_t _connectionId;
        Exchange::IMessageControl* _manager;
        Observer _observer;
    };

} // namespace Plugin
} // namespace WPEFramework
