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

    SERVICE_REGISTRATION(Messenger, 1, 0);

    // IPlugin methods

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
        _service->Register(&_notification);

        _roomAdmin = service->Root<Exchange::IRoomAdministrator>(_connectionId, 2000, _T("RoomMaintainer"));
        if(_roomAdmin == nullptr) {
            message = _T("RoomMaintainer couldnt be instantiated");
        }
        else {
            RegisterAll();
            _roomAdmin->Register(this);
        }

        if(message.length() != 0) {
            Deinitialize(service);
        }
        return message;
    }

    /* virtual */ void Messenger::Deinitialize(PluginHost::IShell* service)
    {
        ASSERT(service == _service);

        _service->Unregister(&_notification);

        if(_roomAdmin != nullptr) {
            // Exit all the rooms (if any) that were joined by this client
            for (auto& room : _roomIds) {
                room.second->Release();
            }

            _roomIds.clear();
            _roomAdmin->Unregister(this);
            _rooms.clear();
            UnregisterAll();

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

    // Web request handlers

    string Messenger::JoinRoom(const string& roomName, const string& userName)
    {
        bool result = false;

        string roomId = GenerateRoomId(roomName, userName);

        MsgNotification* sink = Core::Service<MsgNotification>::Create<MsgNotification>(this, roomId);
        ASSERT(sink != nullptr);

        if (sink != nullptr) {
            Exchange::IRoomAdministrator::IRoom* room = _roomAdmin->Join(roomName, userName, sink);

            // Note: Join() can return nullptr if the user has already joined the room.
            if (room != nullptr) {

                _adminLock.Lock();
                result = _roomIds.emplace(roomId, room).second;
                _adminLock.Unlock();
                ASSERT(result);
            }

            sink->Release(); // Make room the only owner of the notification object.
        }

        return (result? roomId : string{});
    }

    bool Messenger::SubscribeUserUpdate(const string& roomId, bool subscribe)
    {
        bool result = false;

        _adminLock.Lock();

        auto it(_roomIds.find(roomId));

        if (it != _roomIds.end()) {
            Callback* cb = nullptr;

            if (subscribe) {
                cb = Core::Service<Callback>::Create<Callback>(this, roomId);
                ASSERT(cb != nullptr);
            }

            (*it).second->SetCallback(cb);

            if (cb != nullptr) {
                cb->Release(); // Make room the only owner of the callback object.
            }

            result = true;
        }

        _adminLock.Unlock();

        return result;
    }

    bool Messenger::LeaveRoom(const string& roomId)
    {
        bool result = false;

        _adminLock.Lock();

        auto it(_roomIds.find(roomId));

        if (it != _roomIds.end()) {
            // Exit the room.
            (*it).second->Release();
            // Invalidate the room ID.
            _roomIds.erase(it);
            result = true;
        }

        _adminLock.Unlock();

        return result;
    }

    bool Messenger::SendMessage(const string& roomId, const string& message)
    {
        bool result = false;

        _adminLock.Lock();

        auto it(_roomIds.find(roomId));

        if (it != _roomIds.end()) {
            // Send the message to the room.
            (*it).second->SendMessage(message);
            result = true;
        }

        _adminLock.Unlock();

        return result;
    }

    // Helpers

    string Messenger::GenerateRoomId(const string& roomName, const string& userName)
    {
        string timenow;
        Core::Time::Now().ToString(timenow);

        string roomIdBase = roomName + userName + timenow;
        Crypto::SHA1 digest(reinterpret_cast<const uint8_t *>(roomIdBase.c_str()), static_cast<uint16_t>(roomIdBase.length()));

        string roomId;
        Core::ToHexString(digest.Result(), (digest.Length / 2), roomId); // let's take only half of the hash

        return roomId;
    }

    void Messenger::Deactivated(RPC::IRemoteConnection* connection)
    {
        if (connection->Id() == _connectionId) {

            ASSERT(_service != nullptr);

            Core::IWorkerPool::Instance().Submit(PluginHost::IShell::Job::Create(_service,
                PluginHost::IShell::DEACTIVATED,
                PluginHost::IShell::FAILURE));
        }
    }
} // namespace Plugin

} // WPEFramework

