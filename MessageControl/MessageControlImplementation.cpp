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
        Core::SystemInfo::GetEnvironment(Core::MessageUnit::MESSAGE_DISPACTHER_IDENTIFIER_ENV, result);
        return result;
    }

    string DispatcherBasePath()
    {
        string result;
        Core::SystemInfo::GetEnvironment(Core::MessageUnit::MESSAGE_DISPATCHER_PATH_ENV, result);
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
        void Start() override
        {
            _client.AddInstance(0);
            _client.AddFactory(Core::MessageMetaData::MessageType::TRACING, &_factory);
            _worker.Start();

            //check if data is already available
            _client.SkipWaiting();
        }

        void Configure(const bool isBackground, const string& configuration, const string& volatilePath) override
        {
            Config config;
            config.FromString(configuration);
            if (config.Abbreviated.IsSet()) {
                _outputDirector.AbbreviateMessages(config.Abbreviated.Value());
            }
            if ((!isBackground && !config.Console.IsSet() && !config.SysLog.IsSet())
                || (config.Console.IsSet() && config.Console.Value())) {
                _outputDirector.AddOutput(make_unique<ConsoleOutput>());
            }
            if ((isBackground && !config.Console.IsSet() && !config.SysLog.IsSet())
                || (config.SysLog.IsSet() && config.SysLog.Value())) {
                _outputDirector.AddOutput(make_unique<SyslogOutput>());
            }
            if (config.FileName.IsSet()) {
                string fullPath = volatilePath + config.FileName.Value();
                _outputDirector.AddOutput(make_unique<FileOutput>(fullPath));
            }
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
                if (message.Value().first.MetaData().Type() != Core::MessageMetaData::MessageType::INVALID) {
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

        Trace::Factory _factory;
        MessageDirector _outputDirector;
    };

    SERVICE_REGISTRATION(MessageControlImplementation, 1, 0);
}
}
