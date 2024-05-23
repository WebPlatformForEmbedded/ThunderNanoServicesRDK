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
#include "Messenger.h"
#include "cryptalgo/Hash.h"

namespace WPEFramework {

namespace Plugin {

    namespace {

        static Metadata<Messenger> metadata(
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

    /* virtual */ const string Messenger::Initialize(PluginHost::IShell* service)
    {
        string message;

        ASSERT(service != nullptr);
        ASSERT(_service == nullptr);
        ASSERT(_roomAdmin == nullptr);
        ASSERT(_roomIds.empty() == true);
        ASSERT(_rooms.empty() == true);
        ASSERT(_roomACL.empty() == true);
        ASSERT(_connectionId == 0);

        _service = service;
        _service->AddRef();
        _service->Register(static_cast<RPC::IRemoteConnection::INotification*>(&_notification));
        _service->Register(static_cast<PluginHost::IShell::IConnectionServer::INotification*>(&_notification));

        _roomAdmin = service->Root<Exchange::IRoomAdministrator>(_connectionId, 2000, _T("RoomMaintainer"));
        if (_roomAdmin == nullptr) {
            message = _T("RoomMaintainer couldnt be instantiated");
        }
        else {
            _roomAdmin->Register(this);
            JMessenger::Register(*this, this, this);
        }

        return message;
    }

    /* virtual */ void Messenger::Deinitialize(PluginHost::IShell* service VARIABLE_IS_NOT_USED)
    {
        if (_service != nullptr) {
            ASSERT(service == _service);

            JMessenger::Unregister(*this);

            _service->Unregister(static_cast<PluginHost::IShell::IConnectionServer::INotification*>(&_notification));
            _service->Unregister(static_cast<RPC::IRemoteConnection::INotification*>(&_notification));

            if (_roomAdmin != nullptr) {
                // Exit all the rooms (if any) that were joined by this client
                for (auto& room : _roomIds) {
                    room.second.first->Release();
                }

                _roomIds.clear();
                _roomAdmin->Unregister(this);
                _rooms.clear();

                RPC::IRemoteConnection* connection(_service->RemoteConnection(_connectionId));
                VARIABLE_IS_NOT_USED uint32_t result = _roomAdmin->Release();
                _roomAdmin = nullptr;
                // It should have been the last reference we are releasing,
                // so it should end up in a DESCRUCTION_SUCCEEDED, if not we
                // are leaking...
                ASSERT(result == Core::ERROR_DESTRUCTION_SUCCEEDED);

                // If this was running in a (container) proccess...
                if (connection != nullptr) {
                    // Lets trigger the cleanup sequence for
                    // out-of-process code. Which will guard
                    // that unwilling processes, get shot if
                    // not stopped friendly :~)
                    connection->Terminate();
                    connection->Release();
                }
            }

            _service->Release();
            _service = nullptr;
            _connectionId = 0;
            _roomACL.clear();
        }
    }

    /* virtual */ uint32_t Messenger::Join(const Core::JSONRPC::Context& context, const string& roomName, const string& userName,
        const security secure VARIABLE_IS_NOT_USED, IStringIterator* const acl VARIABLE_IS_NOT_USED, string& roomId)
    {
        uint32_t result = Core::ERROR_BAD_REQUEST;

        if ((roomName.empty() == false) && (userName.empty() == false)) {

            roomId = GenerateRoomId(roomName, userName);
            ASSERT(roomId.empty() == false);


            if (sink != nullptr) {
                ASSERT(sink != nullptr);
                MsgNotification* sink = Core::ServiceType<MsgNotification>::Create<MsgNotification>(*this, roomId);

                // Note: Join() can return nullptr if the user has already joined the room.
                if (room != nullptr) {

                    _adminLock.Lock();
                    result = _roomIds.emplace(roomId, std::pair<Exchange::IRoomAdministrator::IRoom*, uint32_t>(room, context.ChannelId())).second;
                    _adminLock.Unlock();
                    ASSERT(result);

                    result = Core::ERROR_NONE;
                }
                else {
                    result = Core::ERROR_ILLEGAL_STATE;
                    roomId.clear();
                }

                sink->Release(); // Make room the only owner of the notification object.
            }
        }

        return result;
    }

    /* virtual */ uint32_t Messenger::Leave(const string& roomId)
    {
        uint32_t result = Core::ERROR_UNKNOWN_KEY;

        _adminLock.Lock();

        auto it(_roomIds.find(roomId));

        if (it != _roomIds.end()) {
            // Exit the room.
            (*it).second.first->Release();
            // Invalidate the room ID.
            _roomIds.erase(it);
            result = Core::ERROR_NONE;
        }

        _adminLock.Unlock();

        return result;
    }

    /* virtual */  uint32_t Messenger::Send(const string& roomId, const string& message)
    {
        uint32_t result = Core::ERROR_UNKNOWN_KEY;

        _adminLock.Lock();

        auto it(_roomIds.find(roomId));

        if (it != _roomIds.end()) {
            // Send the message to the room.
            (*it).second.first->SendMessage(message);
            result = Core::ERROR_NONE;
        }

        _adminLock.Unlock();

        return result;
    }

    bool Messenger::SubscribeUserUpdate(const string& roomId, bool subscribe)
    {
        bool result = false;

        if (roomId.empty() == false) {

            _adminLock.Lock();

            auto it(_roomIds.find(roomId));

            if (it != _roomIds.end()) {
                Callback* cb = nullptr;

                if (subscribe == true) {
                    cb = Core::ServiceType<Callback>::Create<Callback>(*this, roomId);
                    ASSERT(cb != nullptr);
                }

                (*it).second.first->SetCallback(cb);

                if (cb != nullptr) {
                    cb->Release(); // Make room the only owner of the callback object.
                }

                result = true;
            }

            _adminLock.Unlock();
        }

        return result;
    }

    /* static */ string Messenger::GenerateRoomId(const string& roomName, const string& userName)
    {
        ASSERT(roomName.empty() == false);
        ASSERT(userName.empty() == false);

        string timenow;
        Core::Time::Now().ToString(timenow);

        string roomIdBase = roomName + userName + timenow;
        Crypto::SHA1 digest(reinterpret_cast<const uint8_t *>(roomIdBase.c_str()), static_cast<uint16_t>(roomIdBase.length()));

        string roomId;
        Core::ToHexString(digest.Result(), (digest.Length / 2), roomId); // let's take only half of the hash

        return roomId;
    }

    void Messenger::Closed(const uint32_t channel)
    {
        _adminLock.Lock();

        auto it(_roomIds.begin());

        while (it != _roomIds.end()) {

            if ((*it).second.second == channel) {

                // The remote JSON-RPC client was disconnected without leaving the room beforehand!

                TRACE(Trace::Information, (_T("Channel that room ID %s was created on (%u) has been disconnected"),
                    (*it).first.c_str(), channel));

                (*it).second.first->Release();
                it = _roomIds.erase(it);
            }
            else {
                ++it;
            }
        }

        _adminLock.Unlock();
    }

    void Messenger::Deactivated(RPC::IRemoteConnection* connection)
    {
        ASSERT(connection != nullptr);

        if (connection->Id() == _connectionId) {

            ASSERT(_service != nullptr);

            Core::IWorkerPool::Instance().Submit(PluginHost::IShell::Job::Create(_service,
                PluginHost::IShell::DEACTIVATED,
                PluginHost::IShell::FAILURE));
        }
    }

} // namespace Plugin

}

