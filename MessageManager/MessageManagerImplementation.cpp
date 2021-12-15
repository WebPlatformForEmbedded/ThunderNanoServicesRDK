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
        using Job = Core::ThreadPool::JobType<MessageManagerImplementation>;

    public:
        MessageManagerImplementation()
            : _dispatcherIdentifier(DispatcherIdentifier())
            , _dispatcherBasePath(DispatcherBasePath())
            , _client(_dispatcherIdentifier, _dispatcherBasePath)
            , _job(*this)
        {
        }
        ~MessageManagerImplementation() override
        {
            _adminLock.Lock();

            _client.CancelWaiting();
            _job.Revoke();

            _adminLock.Unlock();
        }

        MessageManagerImplementation(const MessageManagerImplementation&) = delete;
        MessageManagerImplementation& operator=(const MessageManagerImplementation&) = delete;

    public:
        void Start() override
        {
            _client.AddInstance(0);
            _client.AddFactory(Core::MessageInformation::MessageType::TRACING, &_factory);
            _job.Submit();
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

            if (instanceIdList.size() > 0) {
                _job.Submit();
            }

            _adminLock.Unlock();
        }

        BEGIN_INTERFACE_MAP(MessageManagerImplementation)
        INTERFACE_ENTRY(Exchange::IMessageManager)
        END_INTERFACE_MAP

    private:
        friend Core::ThreadPool::JobType<MessageManagerImplementation&>;
        mutable Core::CriticalSection _adminLock;

        string _dispatcherBasePath;
        string _dispatcherIdentifier;
        std::list<uint32_t> instanceIds;
        Core::WorkerPool::JobType<MessageManagerImplementation&> _job;
        Core::MessageClient _client;

        Trace::Factory _factory;
    };

    SERVICE_REGISTRATION(MessageManagerImplementation, 1, 0);
}
}
