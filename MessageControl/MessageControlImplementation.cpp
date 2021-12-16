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
        Core::SystemInfo::GetEnvironment(Core::MessageUnit::MESSAGE_DISPACTHER_IDENTIFIER_ENV, result);
        return result;
    }

    string DispatcherBasePath()
    {
        string result;
        Core::SystemInfo::GetEnvironment(Core::MessageUnit::MESSAGE_DISPATCHER_PATH_ENV, result);
        return result;
    }

}

namespace Plugin {

    class MessageControlImplementation : public Exchange::IMessageControl {
        class WorkerThread : private Core::Thread {
        public:
            WorkerThread(MessageControlImplementation& parent)
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

            MessageControlImplementation& _parent;
        };

    public:
        MessageControlImplementation()
            : _dispatcherIdentifier(DispatcherIdentifier())
            , _dispatcherBasePath(DispatcherBasePath())
            , _worker(*this)
            , _client(_dispatcherIdentifier, _dispatcherBasePath)
        {
        }
        ~MessageControlImplementation() override
        {
            _worker.Stop();
            _client.SkipWaiting();

            _adminLock.Lock();
            _client.ClearInstances();
            _adminLock.Unlock();
        }

        MessageControlImplementation(const MessageControlImplementation&) = delete;
        MessageControlImplementation& operator=(const MessageControlImplementation&) = delete;

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
            _client.AddInstance(id);
        }
        void Deactivated(const uint32_t id) override
        {
            _client.RemoveInstance(id);
        }

        void Dispatch()
        {
            _client.WaitForUpdates(Core::infinite);

            Messaging::MessageClient::Message message;

            do {
                message = _client.Pop();
                if (message.Value().first.Type() != Core::MessageInformation::MessageType::INVALID) {

                    string deserialized;
                    std::stringstream output;
                    message.Value().second->ToString(deserialized);

                    string time(Core::Time::Now().ToRFC1123(true));
                    output << '[' << time.c_str() << "]:[" << Core::FileNameOnly(message.Value().first.FileName().c_str()) << ':' << message.Value().first.LineNumber() << "] "
                           << message.Value().first.Category() << ": " << deserialized << std::endl;

                    std::cout << output.str() << std::endl;
                }
            } while (!message.IsSet());
        }

        BEGIN_INTERFACE_MAP(MessageControlImplementation)
        INTERFACE_ENTRY(Exchange::IMessageControl)
        END_INTERFACE_MAP

    private:
        mutable Core::CriticalSection _adminLock;
        string _dispatcherIdentifier;
        string _dispatcherBasePath;
        WorkerThread _worker;
        Messaging::MessageClient _client;

        Trace::Factory _factory;
    };

    SERVICE_REGISTRATION(MessageControlImplementation, 1, 0);
}
}
