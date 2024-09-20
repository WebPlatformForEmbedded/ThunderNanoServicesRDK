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
#include "RoomMaintainer.h"
#include "RoomImpl.h"

namespace Thunder {

namespace Plugin {

    SERVICE_REGISTRATION(RoomMaintainer, 1, 0)

    /* virtual */ Exchange::IRoomAdministrator::IRoom* RoomMaintainer::Join(const string& roomId, const string& userId,
        Exchange::IRoomAdministrator::IRoom::IMsgNotification* messageSink)
    {
        // Note: Nullptr message sink is allowed (e.g. for broadcast-only users).

        Exchange::IRoomAdministrator::IRoom* newRoomUser = nullptr;

        _adminLock.Lock();

        auto it(_rooms.find(roomId));

        if (it == _rooms.end()) {
            // Room not found, so create one.
            it = _rooms.emplace(std::piecewise_construct, std::forward_as_tuple(roomId), std::forward_as_tuple()).first;

            TRACE(Trace::Information, (_T("Room Maintainer: Room '%s' created"), roomId.c_str()));
            if (roomId.size() == 0) {
                TRACE(Trace::Warning, (_T("Room Maintainer: Created a room with empty roomId")));
            }

            // Notify the observers about a new room.
            for (auto& observer : _observers) {
                observer->Created(roomId);
            }
        }

        ASSERT(it != _rooms.end());

        bool foundUser(false);

        it->second.Visit([&userId, &foundUser](Core::ProxyType<Exchange::IRoomAdministrator::IRoom>& entry) -> bool {
            const string& entryUserId = reinterpret_cast<RoomImpl*>(entry.operator->())->UserId();
            return foundUser = ((userId.size() == entryUserId.size()) && (entryUserId.compare(userId) == 0));
        });

        if (foundUser == false) {
            // Add a client to the room;
            Core::ProxyType<Exchange::IRoomAdministrator::IRoom> client;
            
            client = it->second.Instance<RoomImpl>(this, roomId, userId, messageSink);

            if (client.IsValid() == true) {
                client.AddRef();

                newRoomUser = client.operator->();

                it->second.Visit([&userId](Core::ProxyType<Exchange::IRoomAdministrator::IRoom>& entry) -> bool {
                    reinterpret_cast<RoomImpl*>(entry.operator->())->UserJoined(userId);
                    return false;
                });

                if (newRoomUser) {
                    TRACE(Trace::Information, (_T("Room Maintainer: User '%s' has joined room '%s'"), userId.c_str(), roomId.c_str()));
                }
            }
        }

        _adminLock.Unlock();

        // May be nullptr if the user has already joined the room earlier.
        return newRoomUser;
    }

    void RoomMaintainer::Exit(const RoomImpl* roomUser)
    {
        ASSERT(roomUser != nullptr);

        _adminLock.Lock();

        auto it(_rooms.find(roomUser->RoomId()));
        ASSERT(it != _rooms.end());

        if (it != _rooms.end()) {
            // Notify the room members about a leaving user.
            it->second.Visit([&](Core::ProxyType<Exchange::IRoomAdministrator::IRoom>& entry) -> bool {
                reinterpret_cast<RoomImpl*>(entry.operator->())->UserLeft(roomUser->UserId());
                return false;
            });

            // Was it the last user?
            if (it->second.Count() == 0) {
                _rooms.erase(it);

                TRACE(Trace::Information, (_T("Room Maintainer: Room '%s' has been destroyed"), roomUser->RoomId().c_str()));

                // Notify the observers about the destruction of this room.
                for (auto& observer : _observers) {
                    observer->Destroyed(roomUser->RoomId());
                }
            }
        }

        _adminLock.Unlock();
    }

    void RoomMaintainer::Notify(RoomImpl* roomUser)
    {
        ASSERT(roomUser != nullptr);

        _adminLock.Lock();

        auto it = _rooms.find(roomUser->RoomId());
        ASSERT(it != _rooms.end());

        if (it != _rooms.end()) {
            it->second.Visit([&](Core::ProxyType<Exchange::IRoomAdministrator::IRoom>& entry) -> bool {
                roomUser->UserJoined(reinterpret_cast<RoomImpl*>(entry.operator->())->UserId());
                return false;
            });
        }

        _adminLock.Unlock();
    }

    void RoomMaintainer::Send(const string& message, RoomImpl* roomUser)
    {
        ASSERT(roomUser != nullptr);

        _adminLock.Lock();

        auto it(_rooms.find(roomUser->RoomId()));
        ASSERT(it != _rooms.end());

        if (it != _rooms.end()) {

            it->second.Visit([&](Core::ProxyType<Exchange::IRoomAdministrator::IRoom>& entry) -> bool {
                reinterpret_cast<RoomImpl*>(entry.operator->())->MessageReceived(roomUser->UserId(), message);
                return false;
            });
        }

        _adminLock.Unlock();
    }

    /* virtual */ void RoomMaintainer::Register(INotification* sink)
    {
        ASSERT(sink != nullptr);

        _adminLock.Lock();

        auto const it(std::find(_observers.cbegin(), _observers.cend(), sink));

        // Make sure it's not registered multiple times.
        ASSERT(it == _observers.cend());

        if (it == _observers.cend()) {
            _observers.push_back(sink);
            sink->AddRef();

            // Notify the caller about all rooms created to date.
            for (auto const& room : _rooms) {
                sink->Created(room.first);
            }
        }

        _adminLock.Unlock();

        TRACE(Trace::Information, (_T("Room Maintainer: Registered a notification sink")));
    }

    /* virtual */ void RoomMaintainer::Unregister(const INotification* sink)
    {
        ASSERT(sink != nullptr);

        _adminLock.Lock();

        auto const it(std::find(_observers.cbegin(), _observers.cend(), sink));

        // Make sure it was really registered.
        ASSERT(it != _observers.cend());

        if (it != _observers.cend()) {
            (*it)->Release();
            _observers.erase(it);
        }

        _adminLock.Unlock();

        TRACE(Trace::Information, (_T("Room Maintainer: Unregistered a notification sink")));
    }

} // namespace Plugin

} // namespace Thunder
