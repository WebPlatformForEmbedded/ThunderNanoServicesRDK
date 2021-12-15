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

namespace WPEFramework {
namespace {
    string DispatcherIdentifier()
    {
        string result;
        Core::SystemInfo::GetEnvironment(Core::MESSAGE_DISPACTHER_IDENTIFIER_ENV, result);
        return result;
    }

    string DispatcherBasePath()
    {
        string result;
        Core::SystemInfo::GetEnvironment(Core::MESSAGE_DISPATCHER_PATH_ENV, result);
        return result;
    }

}

namespace Plugin {

    class MessageManagerImplementation : public Exchange::IMessageManager {
        class WorkerThread : private Core::Thread {
        public:
            WorkerThread(MessageManagerImplementation& parent)
                : Core::Thread()
                , _parent(parent)
            {
            }
            void Start()
            {
                Thread::Run();
            }
            void Stop()
            {
                Thread::Block();
            }

        private:
            uint32_t Worker() override
            {
                if (Thread::IsRunning()) {
                    _parent.Dispatch();
                }

                return Core::infinite;
            }

            MessageManagerImplementation& _parent;
        };

    public:
        MessageManagerImplementation()
            : _dispatcherIdentifier(DispatcherIdentifier())
            , _dispatcherBasePath(DispatcherBasePath())
            , _client(_dispatcherIdentifier, _dispatcherBasePath)
            , _worker(*this)
        {
        }
        ~MessageManagerImplementation() override
        {
            _worker.Stop();
            _client.SkipWaiting();
            
            _adminLock.Lock();
            _client.ClearInstances();
            _adminLock.Unlock();
        }

        MessageManagerImplementation(const MessageManagerImplementation&) = delete;
        MessageManagerImplementation& operator=(const MessageManagerImplementation&) = delete;

    public:
        void Start() override
        {
            _client.AddInstance(0);
            _client.AddFactory(Core::MessageInformation::MessageType::TRACING, &_factory);
            _worker.Start();

            //check if data is already available
            _client.SkipWaiting();
        }

        void Activated(const uint32_t id) override
        {
            Core::SafeSyncType<Core::CriticalSection> guard(_adminLock);
            _client.AddInstance(id);
        }
        void Deactivated(const uint32_t id) override
        {
            Core::SafeSyncType<Core::CriticalSection> guard(_adminLock);
            _client.RemoveInstance(id);
        }

        void Dispatch()
        {
            _adminLock.Lock();

            _client.WaitForUpdates(Core::infinite);
            auto instanceIdList = _client.InstanceIds();

            for (const auto& id : instanceIdList) {

                auto result = _client.Pop(id);

                if (result.first.Type() != Core::MessageInformation::MessageType::INVALID) {
                    string message;
                    std::stringstream output;
                    result.second->ToString(message);

                    string time(Core::Time::Now().ToRFC1123(true));
                    output << '[' << time.c_str() << "]:[" << Core::FileNameOnly(result.first.FileName().c_str()) << ':' << result.first.LineNumber() << "] "
                           << result.first.Category() << ": " << message << std::endl;

                    std::cout << output.str() << std::endl;
                }
            }

            _adminLock.Unlock();
        }

        BEGIN_INTERFACE_MAP(MessageManagerImplementation)
        INTERFACE_ENTRY(Exchange::IMessageManager)
        END_INTERFACE_MAP

    private:
        mutable Core::CriticalSection _adminLock;
        string _dispatcherIdentifier;
        string _dispatcherBasePath;
        WorkerThread _worker;
        Messaging::MessageClient _client;


        Trace::Factory _factory;
    };

    SERVICE_REGISTRATION(MessageManagerImplementation, 1, 0);
}
}
