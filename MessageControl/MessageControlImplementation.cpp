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

#include "MessageOutput.h"
#include "Module.h"

namespace WPEFramework {
namespace {
    string DispatcherIdentifier()
    {
        string result;
        Core::SystemInfo::GetEnvironment(Core::Messaging::MessageUnit::MESSAGE_DISPACTHER_IDENTIFIER_ENV, result);
        return result;
    }

    string DispatcherBasePath()
    {
        string result;
        Core::SystemInfo::GetEnvironment(Core::Messaging::MessageUnit::MESSAGE_DISPATCHER_PATH_ENV, result);
        return result;
    }

    template <typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
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

        class Config : public Core::JSON::Container {
        private:
            Config(const Config&);
            Config& operator=(const Config&);

        public:
            Config()
                : Core::JSON::Container()
                , FileName()
                , Console(true)
                , SysLog(true)
                , Abbreviated(true)
            {
                Add(_T("console"), &Console);
                Add(_T("syslog"), &SysLog);
                Add(_T("filepath"), &FileName);
                Add(_T("abbreviated"), &Abbreviated);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::String FileName;
            Core::JSON::Boolean Console;
            Core::JSON::Boolean SysLog;
            Core::JSON::Boolean Abbreviated;
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

            _client.ClearInstances();
        }

        MessageControlImplementation(const MessageControlImplementation&) = delete;
        MessageControlImplementation& operator=(const MessageControlImplementation&) = delete;

    public:
        uint32_t Configure(PluginHost::IShell* service) override
        {
            ASSERT(service != nullptr);
            uint32_t result = Core::ERROR_GENERAL;
            if (service != nullptr) {

                Config config;
                config.FromString(service->ConfigLine());

                auto isBackground = service->Background();
                bool abbreviate = false;
                
                if (config.Abbreviated.IsSet()) {
                    abbreviate = config.Abbreviated.Value();
                }
                if ((!isBackground && !config.Console.IsSet() && !config.SysLog.IsSet())
                    || (config.Console.IsSet() && config.Console.Value())) {
                    _outputDirector.AddOutput(make_unique<Messaging::ConsoleOutput>(abbreviate));
                }
                if ((isBackground && !config.Console.IsSet() && !config.SysLog.IsSet())
                    || (config.SysLog.IsSet() && config.SysLog.Value())) {
                    _outputDirector.AddOutput(make_unique<Messaging::SyslogOutput>(abbreviate));
                }
                if (config.FileName.IsSet()) {
                    string fullPath = service->VolatilePath() + config.FileName.Value();
                    _outputDirector.AddOutput(make_unique<Messaging::FileOutput>(abbreviate, fullPath));
                }

                _client.AddInstance(0);
                _client.AddFactory(Core::Messaging::MetaData::MessageType::TRACING, &_factory);
                _worker.Start();

                //check if data is already available
                _client.SkipWaiting();
                result = Core::ERROR_NONE;
            }

            return result;
        }

        void RegisterConnection(const uint32_t id) override
        {
            _client.AddInstance(id);
        }
        void UnregisterConnection(const uint32_t id) override
        {
            _client.RemoveInstance(id);
        }

        uint32_t EnableMessage(MessageType type, const string& moduleName, const string& categoryName, const bool enable) override
        {
            Core::Messaging::MetaData metaData(static_cast<Core::Messaging::MetaData::MessageType>(type), categoryName, moduleName);
            _client.Enable(metaData, enable);

            return Core::ERROR_NONE;
        }

        /**
         * @brief Retreive currently enabled messages information.
         * 
         * @param initialize should the data be fetch (first call)
         * @param type type of the message
         * @param moduleName module 
         * @param categoryName category 
         * @param enable is enabled
         * @return uint32_t ERROR_NONE: Data still available in a list - one should call this method again (but with initialize = false)
         *                  ERROR_UNAVAILABLE: No more data available
         *                  
         */
        uint32_t ActiveMessages(const bool initialize, MessageType& type, string& moduleName, string& categoryName, bool& enable) override
        {
            uint32_t result = Core::ERROR_UNAVAILABLE;

            if (initialize) {
                _controls.Reset(0);
                _controls = _client.Enabled();
            }

            if (_controls.Count() > 0) {
                bool hasNext = _controls.Next();
                if (hasNext) {
                    type = static_cast<MessageType>(_controls.Current().first.Type());
                    moduleName = _controls.Current().first.Module();
                    categoryName = _controls.Current().first.Category();
                    enable = _controls.Current().second;
                    result = Core::ERROR_NONE;
                }
            }

            return result;
        }

        void RegisterOutputNotification(Exchange::IMessageControl::INotification* notification) override
        {
            _outputDirector.RegisterRawMessageNotification(notification);
        }
        void UnregisterOutputNotification(const Exchange::IMessageControl::INotification* notification) override
        {
            _outputDirector.UnregisterRawMessageNotification(notification);
        }

        void Dispatch()
        {
            _client.WaitForUpdates(Core::infinite);

            Messaging::MessageClient::Message message;

            do {
                message = _client.Pop();
                if (message.Value().first.MessageMetaData().Type() != Core::Messaging::MetaData::MessageType::INVALID) {
                    _outputDirector.Output(message.Value().first, message.Value().second.Origin());
                }
            } while (message.IsSet());
        }

        BEGIN_INTERFACE_MAP(MessageControlImplementation)
        INTERFACE_ENTRY(Exchange::IMessageControl)
        END_INTERFACE_MAP

    private:
        string _dispatcherIdentifier;
        string _dispatcherBasePath;
        WorkerThread _worker;
        Messaging::MessageClient _client;

        Messaging::TraceFactory _factory;
        Messaging::MessageDirector _outputDirector;
        Core::Messaging::ControlList::InformationIterator _controls;

        Core::CriticalSection _adminLock;
    };

    SERVICE_REGISTRATION(MessageControlImplementation, 1, 0);
}
}
