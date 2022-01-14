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

#include "MessageControl.h"

namespace WPEFramework {
namespace Plugin {

    SERVICE_REGISTRATION(MessageControl, 1, 0);

    MessageControl::Observer::Observer(MessageControl& parent)
        : _adminLock()
        , _parent(parent)
        , _activationIds()
        , _deactivationIds()
        , _job(*this)
    {
    }

    void MessageControl::Observer::Activated(RPC::IRemoteConnection* connection)
    {
        ASSERT(connection != nullptr);
        Core::SafeSyncType<Core::CriticalSection> guard(_adminLock);
        _activationIds.push_back(connection->Id());
        _job.Submit();
    }
    void MessageControl::Observer::Deactivated(RPC::IRemoteConnection* connection)
    {
        ASSERT(connection != nullptr);
        Core::SafeSyncType<Core::CriticalSection> guard(_adminLock);
        _deactivationIds.push_back(connection->Id());
        _job.Submit();
    }

    void MessageControl::Observer::Dispatch()
    {
        _adminLock.Lock();

        while (_activationIds.size() > 0) {
            _parent.Activated(_deactivationIds.back());
            _activationIds.pop_back();
        }

        while (_deactivationIds.size() > 0) {
            _parent.Deactivated(_deactivationIds.back());
            _deactivationIds.pop_back();
        }

        _adminLock.Unlock();
    }

    MessageControl::MessageControl()
        : _connectionId(0)
        , _observer(*this)
    {
        RegisterAll();
    }

    MessageControl::~MessageControl()
    {
        UnregisterAll();
    }

    const string MessageControl::Initialize(PluginHost::IShell* service)
    {
        ASSERT(service != nullptr);
        string message;

        _control = service->Root<Exchange::IMessageControl>(_connectionId, RPC::CommunicationTimeOut, _T("MessageControlImplementation"));
        if (_control == nullptr) {
            message = _T("MessageControl plugin could not be instantiated.");

        } else {

            _control->Configure(service->Background(), service->ConfigLine(), service->VolatilePath());
            service->Register(&_observer);
        }

        return message;
    }

    void MessageControl::Deinitialize(PluginHost::IShell* service)
    {
        ASSERT(service != nullptr);
        ASSERT(_control != nullptr);
        service->Unregister(&_observer);

        if (_control != nullptr) {
            _control->Release();
            _control = nullptr;
        }
        _connectionId = 0;
    }

    string MessageControl::Information() const
    {
        // No additional info to report.
        return (string());
    }

    void MessageControl::Activated(const uint32_t id)
    {
        ASSERT(_control != nullptr);
        _control->RegisterConnection(id);
    }
    void MessageControl::Deactivated(const uint32_t id)
    {
        ASSERT(_control != nullptr);
        _control->UnregisterConnection(id);
    }

} // namespace Plugin
} // namespace WPEFramework
