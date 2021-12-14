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

#include "EventManager.h"

namespace WPEFramework {
namespace Plugin {

    SERVICE_REGISTRATION(EventManager, 1, 0);

    const string EventManager::Initialize(PluginHost::IShell* service)
    {
        ASSERT(service != nullptr);
        string message;

        _manager = service->Root<Exchange::IEventManager>(_connectionId, RPC::CommunicationTimeOut, _T("EventManagerImplementation"));
        service->Register(&_observer);

        if (_manager == nullptr) {
            std::cerr << _T("EventManager plugin could not be instantiated.") << std::endl;

        } else {
            std::cerr << "INITIALIZED" << std::endl;
            _manager->Start();
        }

        return message;
    }

    void EventManager::Deinitialize(PluginHost::IShell* service)
    {
        std::cerr << "DEINITIALIZE" << std::endl;
        ASSERT(service != nullptr);
        ASSERT(_manager != nullptr);
        service->Unregister(&_observer);

        if (_manager != nullptr) {
            std::cerr << "BEFORE STOP" << std::endl;
            //_manager->Stop();
            _manager->Release();
            std::cerr << "AFTER STOP" << std::endl;
            _manager = nullptr;
        }
        _connectionId = 0;
    }

    string EventManager::Information() const
    {
        // No additional info to report.
        return (string());
    }

} // namespace Plugin
} // namespace WPEFramework
