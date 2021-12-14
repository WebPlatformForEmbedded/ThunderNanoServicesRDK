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
    class EventManagerImplementation : public Exchange::IEventManager {
    public:
        class Worker : public Core::IDispatch {
        public:
            Worker(EventManagerImplementation* parent)
                : _parent(*parent)
            {
            }

            void Dispatch() override
            {
                std::cerr << "WORKER TID: " << std::this_thread::get_id() << std::endl;

                _parent.Dispatch();
                Core::IWorkerPool::Instance().Schedule(Core::Time::Now(), Core::ProxyType<Core::IDispatch>(*this));
            }

        private:
            EventManagerImplementation& _parent;
        };

        EventManagerImplementation()
            : _worker(Core::ProxyType<Worker>::Create(this))
            , _unit(Core::EventUnit::Instance())
        {
        }
        ~EventManagerImplementation() override
        {
            std::cerr << "Stop begin" << std::endl;
            _adminLock.Lock();

            Core::IWorkerPool::Instance().Revoke(Core::ProxyType<Core::IDispatch>(_worker), Core::infinite);
            _buffers.clear();
            _unit.Ring();

            _adminLock.Unlock();
            std::cerr << "Stop end" << std::endl;
        }

        EventManagerImplementation(const EventManagerImplementation&) = delete;
        EventManagerImplementation& operator=(const EventManagerImplementation&) = delete;

    public:
        void Start() override
        {
            DispatcherInfo(_dispatcherIdentifier, _dispatcherBasePath);
            Core::IWorkerPool::Instance().Schedule(Core::Time::Now(), Core::ProxyType<Core::IDispatch>(_worker));

            std::cerr << _dispatcherIdentifier << " " << _dispatcherBasePath << std::endl;
            //messages from wpeframework are of id = 0
            _buffers.emplace(std::piecewise_construct,
                std::forward_as_tuple(0),
                std::forward_as_tuple(_T("md"), 0, false, _T("/tmp/MessageDispatcher")));

            _unit.Announce(Core::EventInformation::EventType::TRACING, &_factory);
        }

        void Stop() override
        {
        }

        void Activated(const uint32_t id) override
        {
            std::cerr << "ACTIVATED" << std::endl;
            _adminLock.Lock();

            ASSERT(_buffers.find(id) == _buffers.end());

            if (_buffers.find(id) == _buffers.end()) {
                std::cerr << _dispatcherIdentifier << " " << _dispatcherBasePath << std::endl;
                _buffers.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(_T("md"), id, false, _T("/tmp/MessageDispatcher")));
            }

            _adminLock.Unlock();
        }
        void Deactivated(const uint32_t id) override
        {
            std::cerr << "DEACTIVATED BEGIN" << std::endl;

            _adminLock.Lock();

            auto index = _buffers.find(id);

            if (index != _buffers.end()) {
                _buffers.erase(index);
            }

            _adminLock.Unlock();

            std::cerr << "DEACTIVATED END" << std::endl;
        }

        void Dispatch()
        {
            std::cerr << "TID: " << std::this_thread::get_id() << std::endl;
            _unit.WaitForUpdates(Core::infinite);
            std::cerr << "DISPATCH" << std::endl;

            for (auto buffer = _buffers.begin(); buffer != _buffers.end(); ++buffer) {

                _adminLock.Lock();

                auto result = _unit.Pop(buffer->second);

                std::cerr << "BUFFER" << std::endl;

                if (result.first.Type() != Core::EventInformation::EventType::INVALID) {
                    string message;
                    result.second->ToString(message);
                    std::cout << message << std::endl;
                }

                _adminLock.Unlock();
            }
        }

        BEGIN_INTERFACE_MAP(EventManagerImplementation)
        INTERFACE_ENTRY(Exchange::IEventManager)
        END_INTERFACE_MAP

    private:
        void DispatcherInfo(string& outIdentifer, string& outBasePath)
        {
            outIdentifer = _T("md");
            outBasePath = _T("/tmp/MessageDispatcher");
            //Core::SystemInfo::GetEnvironment(Core::MESSAGE_DISPATCHER_PATH_ENV, outBasePath);
            //Core::SystemInfo::GetEnvironment(Core::MESSAGE_DISPACTHER_IDENTIFIER_ENV, outIdentifer);
        }

    private:
        string _dispatcherBasePath;
        string _dispatcherIdentifier;
        Core::ProxyType<Worker> _worker;
        std::unordered_map<uint32_t, Core::EventUnit::MessageDispatcher> _buffers;
        Core::CriticalSection _adminLock;
        Core::EventUnit& _unit;

        Trace::Factory _factory;
    };

    SERVICE_REGISTRATION(EventManagerImplementation, 1, 0);
}
}
