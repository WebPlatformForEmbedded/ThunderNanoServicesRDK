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

#include "MessageManager.h"

namespace WPEFramework {
namespace Plugin {

    SERVICE_REGISTRATION(MessageManager, 1, 0);

    const string MessageManager::Initialize(PluginHost::IShell* service)
    {
        ASSERT(service != nullptr);
        string message;

        _manager = service->Root<Exchange::IMessageManager>(_connectionId, RPC::CommunicationTimeOut, _T("MessageManagerImplementation"));
        service->Register(&_observer);

        if (_manager == nullptr) {
            message = _T("MessageManager plugin could not be instantiated.");

        } else {
            _manager->Start();
        }

        return message;
    }

    void MessageManager::Deinitialize(PluginHost::IShell* service)
    {
        ASSERT(service != nullptr);
        ASSERT(_manager != nullptr);
        service->Unregister(&_observer);

        if (_manager != nullptr) {
            _manager->Release();
            _manager = nullptr;
        }
        _connectionId = 0;
    }

    string MessageManager::Information() const
    {
        // No additional info to report.
        return (string());
    }

} // namespace Plugin
} // namespace WPEFramework
