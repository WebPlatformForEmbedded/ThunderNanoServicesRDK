/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 RDK Management
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

#include "MessageOutput.h"
#include "Module.h"
#include <functional>

namespace WPEFramework {

namespace Plugin {

    class MessageControlImplementation : public Exchange::IMessageControl, public Exchange::IMessageControl::ICollect {
    private:
        using Cleanups  = std::vector<uint32_t>;

        class WorkerThread : public Core::Thread {
        public:
            WorkerThread() = delete;
            WorkerThread(const WorkerThread&) = delete;
            WorkerThread& operator= (const WorkerThread&) = delete;

            WorkerThread(MessageControlImplementation& parent)
                : Core::Thread()
                , _parent(parent)
            {
            }
            ~WorkerThread() override = default;

        private:
            uint32_t Worker() override
            {
                _parent.Dispatch();

                return (Core::infinite);
            }

        private:
            MessageControlImplementation& _parent;
        };

    public:
        MessageControlImplementation(const MessageControlImplementation&) = delete;
        MessageControlImplementation& operator=(const MessageControlImplementation&) = delete;

        MessageControlImplementation()
            : _adminLock()
            , _callback(nullptr)
            , _dispatcherIdentifier(Messaging::MessageUnit::Instance().Identifier())
            , _dispatcherBasePath(Messaging::MessageUnit::Instance().BasePath())
            , _worker(*this)
            , _client(_dispatcherIdentifier, _dispatcherBasePath, Messaging::MessageUnit::Instance().SocketPort())
            , _factory()
        {
            _client.AddInstance(0);
            _client.AddFactory(Core::Messaging::Metadata::type::TRACING, &_factory);
            _client.AddFactory(Core::Messaging::Metadata::type::LOGGING, &_factory);
        }

        ~MessageControlImplementation() override
        {
            ASSERT(_callback == nullptr);

            _worker.Stop();
            _worker.Wait(Core::Thread::STOPPED, Core::infinite);
            _client.ClearInstances();
        }

    public:
        uint32_t Callback(Exchange::IMessageControl::ICollect::ICallback* callback) override
        {
            _adminLock.Lock();

            ASSERT((_callback != nullptr) ^ (callback != nullptr));

            if (_callback != nullptr) {

                _worker.Block();
                _client.SkipWaiting();

                _adminLock.Unlock();

                _worker.Wait(Core::Thread::BLOCKED, Core::infinite);

                _adminLock.Lock();

                _callback->Release();
            }
            _callback = callback;

            if (_callback != nullptr) {
                _callback->AddRef();
                _worker.Run();
            }

            _adminLock.Unlock();

            return (Core::ERROR_NONE);
        }

        uint32_t Attach(const uint32_t id) override
        {
            _adminLock.Lock();
            _client.AddInstance(id);
            _adminLock.Unlock();

            Cleanup();

            return (Core::ERROR_NONE);
        }

        uint32_t Detach(const uint32_t id) override
        {
            _adminLock.Lock();
            _client.RemoveInstance(id);
            _adminLock.Unlock();

            // There are "Voll idioten" that where allowed to develop software. They assume that what
            // you bring up should never be brought down and if you want to bring it down, you just
            // shoot-to-kill, no need to allow for a proper shutdown. A-symetrical interfaces allowed
            // and used here!
            // If you have to work with these briljant minds you need to come up with a backup plan.
            // So besides assuming that there is a proper cleanup on destruction assume the worst,
            // which might happen occasionally anyway, BUT NOT BY DEFAULT!
            // So this cleanup seems superflous but it definitly is not!!!
            _cleaning.emplace_back(id);

            Cleanup();

            return (Core::ERROR_NONE);
        }

        void Cleanup() {

            // Also have a list through the cleanup we should do, just to make sure
            Cleanups::iterator index = _cleaning.begin();
            while (index != _cleaning.end()) {
                bool destructed = true;
                string filter (_dispatcherIdentifier + '.' + Core::NumberType<uint32_t>(*index).Text() + _T(".*"));
                Core::Directory directory (_dispatcherBasePath.c_str(), filter.c_str());

                while ((destructed == true) && (directory.Next() == true)) {
                    Core::File file (directory.Current());
                    destructed = destructed && (Core::File(directory.Current()).Destroy());
                }

                if (destructed == true) {
                    index = _cleaning.erase(index); 
                }
                else {
                    index++;
                }
            }
        }

        uint32_t Enable(const messagetype type, const string& category, const string& module, const bool enabled) override
        {
            _client.Enable({static_cast<Core::Messaging::Metadata::type>(type), category, module}, enabled);

            return (Core::ERROR_NONE);
        }

        uint32_t Controls(Exchange::IMessageControl::IControlIterator*& controls) const override
        {
            std::list<Exchange::IMessageControl::Control> list;
            Messaging::MessageUnit::Iterator index;

            _client.Controls(index);

            while (index.Next() == true) {
                list.push_back( { static_cast<messagetype>(index.Type()), index.Category(), index.Module(), index.Enabled() } );
            }

            using Implementation = RPC::IteratorType<Exchange::IMessageControl::IControlIterator>;
            controls = Core::Service<Implementation>::Create<Exchange::IMessageControl::IControlIterator>(list);

            return (Core::ERROR_NONE);
        }

    public:
        BEGIN_INTERFACE_MAP(MessageControlImplementation)
            INTERFACE_ENTRY(Exchange::IMessageControl)
            INTERFACE_ENTRY(Exchange::IMessageControl::ICollect)
        END_INTERFACE_MAP

    private:
        void Dispatch()
        {
            _client.WaitForUpdates(Core::infinite);

            _client.PopMessagesAndCall([this](const Core::Messaging::IStore::Information& info, const Core::ProxyType<Core::Messaging::IEvent>& message) {
                ASSERT(_callback != nullptr);

                // Turn data into piecies to trasfer over the wire
                _callback->Message(static_cast<Exchange::IMessageControl::messagetype>(info.Type()), info.Module(), info.Category(), info.FileName(), info.LineNumber(), info.ClassName(), info.TimeStamp(), message->Data());
            });
        }

    private:
        Core::CriticalSection _adminLock;
        IMessageControl::ICollect::ICallback* _callback;

        const string _dispatcherIdentifier;
        const string _dispatcherBasePath;

        WorkerThread _worker;
        Messaging::MessageClient _client;
        Messaging::TraceFactory _factory;

        Cleanups _cleaning;
    };

    SERVICE_REGISTRATION(MessageControlImplementation, 1, 0);

} // namespace Plugin
}
