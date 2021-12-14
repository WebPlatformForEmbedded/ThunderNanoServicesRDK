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
namespace Plugin {
    class MessageManagerImplementation : public Exchange::IMessageManager {
        using Job = Core::ThreadPool::JobType<MessageManagerImplementation>;

    public:
        MessageManagerImplementation()
            : _unit(Core::MessageUnit::Instance())
            , _job(*this)
        {
            DispatcherInfo(_dispatcherIdentifier, _dispatcherBasePath);
        }
        ~MessageManagerImplementation() override
        {
            _adminLock.Lock();

            _buffers.clear();
            _unit.Ring();
            _job.Revoke();

            _adminLock.Unlock();
        }

        MessageManagerImplementation(const MessageManagerImplementation&) = delete;
        MessageManagerImplementation& operator=(const MessageManagerImplementation&) = delete;

    public:
        void Start() override
        {
            _job.Submit();

            //messages from wpeframework are of id = 0
            _buffers.emplace(std::piecewise_construct,
                std::forward_as_tuple(0),
                std::forward_as_tuple(_dispatcherIdentifier, 0, false, _dispatcherBasePath));

            _unit.Announce(Core::MessageInformation::MessageType::TRACING, &_factory);
        }

        void Activated(const uint32_t id) override
        {
            _adminLock.Lock();

            ASSERT(_buffers.find(id) == _buffers.end());

            if (_buffers.find(id) == _buffers.end()) {
                _buffers.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(_dispatcherIdentifier, id, false, _dispatcherBasePath));
            }

            _adminLock.Unlock();
        }
        void Deactivated(const uint32_t id) override
        {
            _adminLock.Lock();

            auto index = _buffers.find(id);

            if (index != _buffers.end()) {
                _buffers.erase(index);
            }

            _adminLock.Unlock();
        }

        void Dispatch()
        {
            _unit.WaitForUpdates(Core::infinite);

            for (auto buffer = _buffers.begin(); buffer != _buffers.end(); ++buffer) {

                _adminLock.Lock();

                auto result = _unit.Pop(buffer->second);

                if (result.first.Type() != Core::MessageInformation::MessageType::INVALID) {
                    string message;
                    std::stringstream output;
                    result.second->ToString(message);

                    string time(Core::Time::Now().ToRFC1123(true));
                    output << '[' << time.c_str() << "]:[" << Core::FileNameOnly(result.first.FileName().c_str()) << ':' << result.first.LineNumber() << "] "
                           << result.first.Category() << ": " << message << std::endl;

                    std::cout << output.str() << std::endl;
                }

                _adminLock.Unlock();
            }

            if (_buffers.size() > 0) {
                _job.Submit();
            }
        }

        BEGIN_INTERFACE_MAP(MessageManagerImplementation)
        INTERFACE_ENTRY(Exchange::IMessageManager)
        END_INTERFACE_MAP

    private:
        void DispatcherInfo(string& outIdentifer, string& outBasePath)
        {
            Core::SystemInfo::GetEnvironment(Core::MESSAGE_DISPATCHER_PATH_ENV, outBasePath);
            Core::SystemInfo::GetEnvironment(Core::MESSAGE_DISPACTHER_IDENTIFIER_ENV, outIdentifer);
        }

    private:
        friend Core::ThreadPool::JobType<MessageManagerImplementation&>;

        string _dispatcherBasePath;
        string _dispatcherIdentifier;
        std::unordered_map<uint32_t, Core::MessageUnit::MessageDispatcher> _buffers;
        Core::CriticalSection _adminLock;
        Core::WorkerPool::JobType<MessageManagerImplementation&> _job;
        Core::MessageUnit& _unit;

        Trace::Factory _factory;
    };

    SERVICE_REGISTRATION(MessageManagerImplementation, 1, 0);
}
}
