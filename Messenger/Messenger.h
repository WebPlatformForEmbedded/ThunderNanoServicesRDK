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
#include <interfaces/IMessenger.h>
#include <interfaces/json/JMessenger.h>
#include <map>
#include <set>
#include <functional>

namespace WPEFramework {

namespace JMessenger = Exchange::JSONRPC::JMessenger;

namespace Plugin {

    class Messenger : public PluginHost::IPlugin
                    , public Exchange::IRoomAdministrator::INotification
                    , public PluginHost::JSONRPCSupportsEventStatus
                    , public Exchange::JSONRPC::JMessenger::IHandler
                    , public Exchange::JSONRPC::IMessenger {

        using IMessenger = Exchange::JSONRPC::IMessenger;

    private:
        class Notification : public RPC::IRemoteConnection::INotification
                           , public PluginHost::IShell::IJSONRPCLink::INotification {
        public:
            Notification() = delete;
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;

            explicit Notification(Messenger& parent)
                : _parent(parent)
            {
            }
            ~Notification() override = default;

        public:
            void Activated(RPC::IRemoteConnection* /* connection */) override
            {
            }
            void Deactivated(RPC::IRemoteConnection* connection) override
            {
                _parent.Deactivated(connection);
            }
            void Terminated(RPC::IRemoteConnection* /* connection */) override
            {
            }

        public:
            void Opened(const uint32_t channel VARIABLE_IS_NOT_USED) override
            {
            }
            void Closed(const uint32_t channel) override
            {
                _parent.Closed(channel);
            }

        public:
            BEGIN_INTERFACE_MAP(Notification)
                INTERFACE_ENTRY(RPC::IRemoteConnection::INotification)
                INTERFACE_ENTRY(PluginHost::IShell::IJSONRPCLink::INotification)
            END_INTERFACE_MAP

        private:
            Messenger& _parent;
        }; // class Notification

    private:
        class MsgNotification : public Exchange::IRoomAdministrator::IRoom::IMsgNotification {
        public:
            MsgNotification(const MsgNotification&) = delete;
            MsgNotification& operator=(const MsgNotification&) = delete;

            MsgNotification(Messenger& parent, const string& roomId)
                : _parent(parent)
                , _roomId(roomId)
            {
                ASSERT(roomId.empty() == false);
            }

            ~MsgNotification() override
            {
                TRACE(Trace::Information,( _T("MsgNotification object for room '%s' destroyed"), _roomId.c_str()));
            }

        public:
            // IRoom::IMsgNotification overrides
            void Message(const string& senderName, const string& message) override
            {
                ASSERT(senderName.empty() == false);
                JMessenger::Event::Message(_parent, _roomId /* index */, senderName, message);
            }

        public:
            BEGIN_INTERFACE_MAP(MsgNotification)
                INTERFACE_ENTRY(Exchange::IRoomAdministrator::IRoom::IMsgNotification)
            END_INTERFACE_MAP

        private:
            Messenger& _parent;
            string _roomId;
        }; // class MsgNotification

    private:
        class Callback : public Exchange::IRoomAdministrator::IRoom::ICallback {
        public:
            Callback(const Callback&) = delete;
            Callback& operator=(const Callback&) = delete;

            Callback(Messenger& parent, const string& roomId)
                : _parent(parent)
                , _roomId(roomId)
            {
                ASSERT(roomId.empty() == false);
            }

            ~Callback()
            {
                TRACE(Trace::Information,( _T("Callback object for room '%s' destroyed"), _roomId.c_str()));
            }

        public:
            // IRoom::ICallback overrides
            void Joined(const string& userName) override
            {
                ASSERT(userName.empty() == false);
                JMessenger::Event::UserUpdate(_parent, _roomId /* index */, userName, IMessenger::INotification::JOINED);
            }
            void Left(const string& userName) override
            {
                ASSERT(userName.empty() == false);
                JMessenger::Event::UserUpdate(_parent, _roomId /* index */, userName, IMessenger::INotification::LEFT);
            }

        public:
            BEGIN_INTERFACE_MAP(Callback)
                INTERFACE_ENTRY(Exchange::IRoomAdministrator::IRoom::ICallback)
            END_INTERFACE_MAP

        private:
            Messenger& _parent;
            string _roomId;
        }; // class Callback

    public:
        Messenger(const Messenger&) = delete;
        Messenger& operator=(const Messenger&) = delete;

PUSH_WARNING(DISABLE_WARNING_THIS_IN_MEMBER_INITIALIZER_LIST)
        Messenger()
            : JSONRPCSupportsEventStatus(std::bind(&Messenger::CheckToken, this,
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
            , _connectionId(0)
            , _service(nullptr)
            , _roomAdmin(nullptr)
            , _roomIds()
            , _rooms()
            , _roomACL()
            , _adminLock()
            , _notification(*this)
        {
        }
POP_WARNING()

        ~Messenger() override = default;

        // IPlugin overrides
        const string Initialize(PluginHost::IShell* service) override;
        void Deinitialize(PluginHost::IShell* service) override;
        string Information() const override  { return { }; }

    public:
        // IRoomAdministrator::INotification overrides
        void Created(const string& roomName) override
        {
            ASSERT(roomName.empty() == false);

            _adminLock.Lock();

            ASSERT(_rooms.find(roomName) == _rooms.end());

            _rooms.insert(roomName);
            JMessenger::Event::RoomUpdate(*this, roomName, IMessenger::INotification::CREATED, IsSecure(roomName));

            _adminLock.Unlock();
        }
        void Destroyed(const string& roomName) override
        {
            ASSERT(roomName.empty() == false);

            _adminLock.Lock();

            ASSERT(_rooms.find(roomName) != _rooms.end());

            JMessenger::Event::RoomUpdate(*this, roomName, IMessenger::INotification::DESTROYED, IsSecure(roomName));

            _rooms.erase(roomName);
            _roomACL.erase(roomName);

            _adminLock.Unlock();
        }

    public:
        // JSONRPC::JMessenger::IHandler overrides
        void OnRoomUpdateEventRegistration(const string& client VARIABLE_IS_NOT_USED, const JSONRPCSupportsEventStatus::Status status) override
        {
            TRACE(Trace::Information, (_T("Client '%s' %s for room-update notification"), client.c_str(),
                status == Status::registered? "registered" : "unregistered"));

            if (status == Status::registered) {
                _adminLock.Lock();

                for (const string& room : _rooms) {
                    JMessenger::Event::RoomUpdate(*this, room, IMessenger::INotification::CREATED, IsSecure(room));
                }

                _adminLock.Unlock();
            }
        }
        void OnUserUpdateEventRegistration(const string& client, const JSONRPCSupportsEventStatus::Status status) override
        {
            if (SubscribeUserUpdate(client.substr(0, client.find('.')), (status == Status::registered)) == true) {

                TRACE(Trace::Information, (_T("Client '%s' %s for user-update notification"), client.c_str(),
                    status == Status::registered? "registered" : "unregistered"));

            } else {
                TRACE(Trace::Error, (_T("Client id '%s' is invalid"), client.c_str()));
            }
        }

    public:
        // JSONRPC::IMessenger overrides
        Core::hresult Join(const Core::JSONRPC::Context& context, const string& roomName, const string& userName,
                           const IMessenger::security secure, IMessenger::IStringIterator* const acl, string& roomId) override;
        Core::hresult Leave(const string& roomId) override;
        Core::hresult Send(const string& roomId, const string& message) override;

    public:
        // QueryInterface implementation
        BEGIN_INTERFACE_MAP(Messenger)
            INTERFACE_ENTRY(PluginHost::IPlugin)
            INTERFACE_ENTRY(Exchange::IRoomAdministrator::INotification)
            INTERFACE_ENTRY(PluginHost::IDispatcher)
            INTERFACE_AGGREGATE(Exchange::IRoomAdministrator, _roomAdmin)
        END_INTERFACE_MAP

    private:
        void Deactivated(RPC::IRemoteConnection* connection);
        void Closed(const uint32_t channel);

        bool SubscribeUserUpdate(const string& roomId, bool subscribe);

        PluginHost::JSONRPC::classification CheckToken(const string& token, const string& method, const string& parameters);

        IMessenger::security IsSecure(const string& room) const {
            return (_roomACL.find(room) != _roomACL.end()? IMessenger::SECURE : IMessenger::INSECURE);
        }

    private:
        static string GenerateRoomId(const string& roomName, const string& userName);

    private:
        uint32_t _connectionId;
        PluginHost::IShell* _service;
        Exchange::IRoomAdministrator* _roomAdmin;
        std::map<string, std::pair<Exchange::IRoomAdministrator::IRoom*, uint32_t>> _roomIds;
        std::set<string> _rooms;
        std::map<string, std::list<string>> _roomACL;
        mutable Core::CriticalSection _adminLock;
        Core::SinkType<Notification> _notification;
    }; // class Messenger

} // namespace Plugin

}
