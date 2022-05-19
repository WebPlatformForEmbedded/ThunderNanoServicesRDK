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

    uint16_t DispatcherSocketPort()
    {
        uint16_t result = 0;
        string socketPort;

        if (Core::SystemInfo::GetEnvironment(Core::Messaging::MessageUnit::MESSAGE_DISPATCHER_SOCKETPORT_ENV, socketPort) == true) {
            result = Core::NumberType<uint16_t>(socketPort.c_str(), static_cast<uint32_t>(socketPort.length()), NumberBase::BASE_DECIMAL).Value();
        }

        return (result);
    }
}

namespace Plugin {

    class MessageControlImplementation : public Exchange::IMessageControl {
    private:
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
                if (Thread::IsRunning()) {
                    _parent.Dispatch();
                }

                return Core::infinite;
            }

        private:
            MessageControlImplementation& _parent;
        };

    public:
        MessageControlImplementation()
            : _dispatcherIdentifier(DispatcherIdentifier())
            , _dispatcherBasePath(DispatcherBasePath())
            , _worker(*this)
            , _client(_dispatcherIdentifier, _dispatcherBasePath, DispatcherSocketPort())
        {
        }
        ~MessageControlImplementation() override
        {
            _worker.Stop();
            _client.SkipWaiting();
            _worker.Wait(Core::Thread::STOPPED, Core::infinite);

            _client.ClearInstances();
        }

        MessageControlImplementation(const MessageControlImplementation&) = delete;
        MessageControlImplementation& operator=(const MessageControlImplementation&) = delete;

    public:
        uint32_t Configure(const bool isBackground, const bool abbreviate, const bool outputToConsole,
            const bool outputToSysLog, const string& outputFileName, const string& binding, const uint32_t port) override
        {
            uint32_t result = Core::ERROR_NONE;

            if ((!isBackground && !outputToConsole && !outputToSysLog) || (outputToConsole)) {
                _outputDirector.AddOutput(Core::Messaging::MetaData::MessageType::TRACING, std::make_shared<Messaging::ConsoleOutput>(abbreviate));
            }
            if ((isBackground && !outputToConsole && !outputToSysLog) || (outputToSysLog)) {
                _outputDirector.AddOutput(Core::Messaging::MetaData::MessageType::TRACING, std::make_shared<Messaging::SyslogOutput>(abbreviate));
            }
            if (!outputFileName.empty()) {
                _outputDirector.AddOutput(Core::Messaging::MetaData::MessageType::TRACING, std::make_shared<Messaging::FileOutput>(abbreviate, outputFileName));
            }
            if (!binding.empty() && port != 0) {
                auto udpOutput = std::make_shared<Messaging::UDPOutput>(Core::NodeId(binding.c_str(), port));
                _outputDirector.AddOutput(Core::Messaging::MetaData::MessageType::TRACING, udpOutput);
                _outputDirector.AddOutput(Core::Messaging::MetaData::MessageType::LOGGING, udpOutput);
            }

            _client.AddInstance(0);
            _client.AddFactory(Core::Messaging::MetaData::MessageType::TRACING, &_factory);
            _client.AddFactory(Core::Messaging::MetaData::MessageType::LOGGING, &_factory);
            _worker.Run();

            //check if data is already available
            _client.SkipWaiting();

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

        uint32_t EnableMessage(const MessageType type, const string& moduleName, const string& categoryName, const bool enable) override
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
            _client.PopMessagesAndCall([this](const Core::Messaging::Information& info, const Core::ProxyType<Core::Messaging::IEvent>& message) {
                _outputDirector.Output(info, message.Origin());
            });
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
