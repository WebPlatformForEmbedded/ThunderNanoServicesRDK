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

#include "MessageControl.h"
#include "MessageOutput.h"

namespace WPEFramework {
namespace Plugin {

    SERVICE_REGISTRATION(MessageControl, 1, 0);

    Core::ProxyPoolType<Messaging::JSONOutput::Data> jsonExportDataFactory(2);
    constexpr uint16_t MAX_CONNECTIONS = 5;

    class WebSocketExporter : public Messaging::IMessageOutput {

    public:
        class ExportCommand : public Core::JSON::Container {

        public:
            ExportCommand(const ExportCommand&) = delete;
            ExportCommand& operator=(const ExportCommand&) = delete;

            ExportCommand()
                : Core::JSON::Container()
                , Filename()
                , Identifier()
                , Category()
                , IncludingDate()
                , Paused()
            {
                Add(_T("filename"), &Filename);
                Add(_T("identifier"), &Identifier);
                Add(_T("category"), &Category);
                Add(_T("includingdate"), &IncludingDate);
                Add(_T("paused"), &Paused);
            }

            ~ExportCommand() override = default;

        public:
            Core::JSON::Boolean Filename;
            Core::JSON::Boolean Identifier;
            Core::JSON::Boolean Category;
            Core::JSON::Boolean IncludingDate;
            Core::JSON::Boolean Paused;
        };

    public:
        class MessageChannelOutput : public Messaging::JSONOutput {

        public:
            MessageChannelOutput(const MessageChannelOutput&) = delete;
            MessageChannelOutput& operator=(const MessageChannelOutput&) = delete;

            explicit MessageChannelOutput(PluginHost::Channel& channel)
                : JSONOutput(channel)
            {
            }
            ~MessageChannelOutput() = default;

            Core::ProxyType<Core::JSON::IElement> Process(const Core::ProxyType<Core::JSON::IElement>& element);
            Core::ProxyType<Data> GetDataContainer() override
            {
                return jsonExportDataFactory.Element();
            }
        };

    public:
        WebSocketExporter(const WebSocketExporter& copy) = delete;
        WebSocketExporter& operator=(const WebSocketExporter&) = delete;

        explicit WebSocketExporter(const uint32_t maxConnections = MAX_CONNECTIONS)
            : _messageChannelOutputs()
            , _lock()
            , _maxExportConnections(maxConnections)
        {
        }

        ~WebSocketExporter() = default;

        bool Activate(PluginHost::Channel& channel)
        {

            bool accepted = false;

            _lock.Lock();

            if ((_maxExportConnections != 0) && (_maxExportConnections - _messageChannelOutputs.size() > 0)) {
                ASSERT(0 == _messageChannelOutputs.count(channel.Id()));
                _messageChannelOutputs.emplace(std::make_pair(channel.Id(), new MessageChannelOutput(channel)));
                accepted = true;
            }

            _lock.Unlock();

            return accepted;
        }

        bool Deactivate(PluginHost::Channel& channel)
        {

            bool deactivated = false;

            _lock.Lock();

            if (_messageChannelOutputs.count(channel.Id() != 0)) {
                _messageChannelOutputs.erase(channel.Id());
                deactivated = true;
            }

            _lock.Unlock();

            return deactivated;
        }

        Core::ProxyType<Core::JSON::IElement> HandleExportCommand(const uint32_t ID, const Core::ProxyType<Core::JSON::IElement>& element);
        void Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message)
        {
            _lock.Lock();

            for (auto& item : _messageChannelOutputs) {
                item.second->Output(info, message);
            }

            _lock.Unlock();
        }

    private:
        std::unordered_map<uint32_t, std::unique_ptr<Messaging::JSONOutput>> _messageChannelOutputs;
        mutable Core::CriticalSection _lock;
        const uint32_t _maxExportConnections;
    };

    static Core::ProxyPoolType<WebSocketExporter::ExportCommand> jsonExportCommandFactory(2);

    Core::ProxyType<Core::JSON::IElement> WebSocketExporter::HandleExportCommand(const uint32_t ID,
        const Core::ProxyType<Core::JSON::IElement>& element)
    {

        Core::ProxyType<Core::JSON::IElement> response;

        _lock.Lock();

        auto index = _messageChannelOutputs.find(ID);
        if (index != _messageChannelOutputs.end()) {
            response = index->second->Process(element);
        }

        _lock.Unlock();

        return response;
    }

    Core::ProxyType<Core::JSON::IElement> WebSocketExporter::MessageChannelOutput::Process(const Core::ProxyType<Core::JSON::IElement>& element)
    {
        Core::ProxyType<WebSocketExporter::ExportCommand> inbound(element);

        ASSERT(inbound.IsValid() == true);

        ExtraOutputOptions options = OutputOptions();

        if (inbound->Filename.IsSet() == true) {
            if (inbound->Filename == true) {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) | AsNumber(ExtraOutputOptions::LINENUMBER));
            } else {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) & ~AsNumber(ExtraOutputOptions::LINENUMBER));
            }
        }

        if (inbound->Identifier.IsSet() == true) {
            if (inbound->Identifier == true) {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) | AsNumber(ExtraOutputOptions::MODULE));
            } else {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) & ~AsNumber(ExtraOutputOptions::MODULE));
            }
        }

        if (inbound->Category.IsSet() == true) {
            if (inbound->Category == true) {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) | AsNumber(ExtraOutputOptions::CATEGORY));
            } else {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) & ~AsNumber(ExtraOutputOptions::CATEGORY));
            }
        }

        if (inbound->IncludingDate.IsSet() == true) {
            if (inbound->IncludingDate == true) {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) | AsNumber(ExtraOutputOptions::INCLUDINGDATE));
            } else {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) & ~AsNumber(ExtraOutputOptions::INCLUDINGDATE));
            }
        }

        OutputOptions(options);

        if (inbound->Paused.IsSet() == true) {
            _paused = inbound->Paused;
        }

        Core::ProxyType<ExportCommand> response(jsonExportCommandFactory.Element());

        response->Filename = ((AsNumber(options) & AsNumber(ExtraOutputOptions::FILENAME)) != 0);
        response->Identifier = ((AsNumber(options) & AsNumber(ExtraOutputOptions::MODULE)) != 0);
        response->Category = ((AsNumber(options) & AsNumber(ExtraOutputOptions::CATEGORY)) != 0);
        response->IncludingDate = ((AsNumber(options) & AsNumber(ExtraOutputOptions::INCLUDINGDATE)) != 0);
        response->Paused = IsPaused();

        return (Core::ProxyType<Core::JSON::IElement>(response));
    }

    MessageControl::Observer::Observer(MessageControl& parent)
        : _adminLock()
        , _parent(parent)
        , _activationIds()
        , _deactivationIds()
        , _job(*this)
    {
    }

    void MessageControl::Observer::Activated(RPC::IRemoteConnection* connection)
    {
        ASSERT(connection != nullptr);
        Core::SafeSyncType<Core::CriticalSection> guard(_adminLock);
        _activationIds.push_back(connection->Id());
        _job.Submit();
    }
    void MessageControl::Observer::Deactivated(RPC::IRemoteConnection* connection)
    {
        ASSERT(connection != nullptr);
        Core::SafeSyncType<Core::CriticalSection> guard(_adminLock);
        _deactivationIds.push_back(connection->Id());
        _job.Submit();
    }

    void MessageControl::Observer::Dispatch()
    {
        _adminLock.Lock();

        while (_activationIds.size() > 0) {
            _parent.Activated(_deactivationIds.back());
            _activationIds.pop_back();
        }

        while (_deactivationIds.size() > 0) {
            _parent.Deactivated(_deactivationIds.back());
            _deactivationIds.pop_back();
        }

        _adminLock.Unlock();
    }

    MessageControl::MessageOutputNotification::MessageOutputNotification(MessageControl& parent)
        : _parent(parent)
    {
    }
    void MessageControl::MessageOutputNotification::ReceiveRawMessage(const Exchange::IMessageControl::MessageType type, const string& category,
        const string& module, const string& fileName,
        const uint16_t lineNumber, const uint64_t timestamp,
        const string& message)
    {
        //yikes, recreating stuff from received pieces
        Messaging::TextMessage textMessage(message);
        Core::Messaging::Information info(static_cast<Core::Messaging::MetaData::MessageType>(type), category, module, fileName, lineNumber, timestamp);
        _parent._webSocketExporter->Output(info, &textMessage);
    }

    MessageControl::ComNotificationSink::ComNotificationSink(MessageControl& parent)
        : _parent(parent)
    {
    }

    void MessageControl::ComNotificationSink::CleanedUp(const Core::IUnknown*, const uint32_t)
    {
    }
    void MessageControl::ComNotificationSink::Revoked(const Core::IUnknown* remote, const uint32_t interfaceId)
    {
        TRACE(Trace::Information, (_T("Revoking an interface: %d [%X] on object: [%s]"), interfaceId, interfaceId, typeid(*remote).name()));

        // Something happened to the other side
        ASSERT(interfaceId != Exchange::ID_MESSAGE_CONTROL);

        if (interfaceId == Exchange::ID_MESSAGE_CONTROL_NOTIFICATION) {
            auto result = remote->QueryInterface<const Exchange::IMessageControl::INotification>();
            ASSERT(result != nullptr);

            _parent.OnRevoke(result);

            result->Release();
        }
    }

    MessageControl::Config::NetworkNode::NetworkNode()
        : Core::JSON::Container()
        , Port(2200)
        , Binding("0.0.0.0")
    {
        Add(_T("port"), &Port);
        Add(_T("binding"), &Binding);
    }
    MessageControl::Config::NetworkNode::NetworkNode(const NetworkNode& copy)
        : Core::JSON::Container()
        , Port(copy.Port)
        , Binding(copy.Binding)
    {
        Add(_T("port"), &Port);
        Add(_T("binding"), &Binding);
    }

    MessageControl::Config::Config()
        : Core::JSON::Container()
        , Console(true)
        , SysLog(true)
        , FileName()
        , Abbreviated(true)
        , MaxExportConnections(1)
        , Remote()
    {
        Add(_T("console"), &Console);
        Add(_T("syslog"), &SysLog);
        Add(_T("filepath"), &FileName);
        Add(_T("abbreviated"), &Abbreviated);
        Add(_T("maxexportconnections"), &MaxExportConnections);
        Add(_T("remote"), &Remote);
    }

    MessageControl::MessageControl()
        : _connectionId(0)
        , _service(nullptr)
        , _observer(*this)
        , _outputNotification(*this)
        , _comSink(*this)
        , _webSocketExporter(nullptr)
        , _fullOutputFilePath(_T(""))
        , _maxExportConnections(1)
    {
    }

    const string MessageControl::Initialize(PluginHost::IShell* service)
    {
        ASSERT(service != nullptr);
        ASSERT(_control == nullptr);
        ASSERT(_service == nullptr);
        ASSERT(_connectionId == 0);

        string message;

        _service = service;
        _service->AddRef();
        _service->Register(&_observer);
        _service->Register(&_comSink);

        Config config;
        config.FromString(service->ConfigLine());
        _fullOutputFilePath = service->VolatilePath() + config.FileName.Value();
        _maxExportConnections = config.MaxExportConnections.IsSet() ? config.MaxExportConnections.Value() : MAX_CONNECTIONS;

        _control = service->Root<Exchange::IMessageControl>(_connectionId, RPC::CommunicationTimeOut, _T("MessageControlImplementation"));
        if (_control == nullptr) {
            message = _T("MessageControl plugin could not be instantiated.");
        } else {
            RegisterAll();
            if (_control->Configure(service->Background(),
                    config.Abbreviated.Value(),
                    config.Console.Value(),
                    config.SysLog.Value(),
                    _fullOutputFilePath,
                    config.Remote.IsSet() ? config.Remote.Binding.Value() : _T(""),
                    config.Remote.IsSet() ? config.Remote.Port.Value() : 0)
                != Core::ERROR_NONE) {
                message = _T("MessageControl plugin could not be instantiated.");
            } else {
                _webSocketExporter.reset(new WebSocketExporter(_maxExportConnections));
                _control->RegisterOutputNotification(&_outputNotification);
            }
        }

        if(message.length() != 0) {
            Deinitialize(service);
        }

        return message;
    }

    void MessageControl::Deinitialize(PluginHost::IShell* service)
    {
        ASSERT(service == _service);

        service->Unregister(&_observer);
        service->Unregister(&_comSink);

        if (_control != nullptr) {
            UnregisterAll();
            _control->UnregisterOutputNotification(&_outputNotification);
            _webSocketExporter.reset();

            RPC::IRemoteConnection* connection(_service->RemoteConnection(_connectionId));

            VARIABLE_IS_NOT_USED uint32_t result = _control->Release();
            _control = nullptr;
            // It should have been the last reference we are releasing,
            // so it should endup in a DESTRUCTION_SUCCEEDED, if not we
            // are leaking...
            ASSERT(result == Core::ERROR_DESTRUCTION_SUCCEEDED);
             // The process can disappear in the meantime...
            if (connection != nullptr) {
                // But if it did not dissapear in the meantime, forcefully terminate it. Shoot to kill :-)
                connection->Terminate();
                connection->Release();
            }
        }

        _service->Release();
        _service = nullptr;
        _connectionId = 0;
    }

    string MessageControl::Information() const
    {
        // No additional info to report.
        return (string());
    }

    void MessageControl::Activated(const uint32_t id)
    {
        ASSERT(_control != nullptr);
        _control->RegisterConnection(id);
    }
    void MessageControl::Deactivated(const uint32_t id)
    {
        ASSERT(_control != nullptr);
        _control->UnregisterConnection(id);
        if (_connectionId == id) {
            ASSERT(_service != nullptr);
            Core::IWorkerPool::Instance().Submit(PluginHost::IShell::Job::Create(_service, PluginHost::IShell::DEACTIVATED, PluginHost::IShell::FAILURE));
        }
    }

    void MessageControl::OnRevoke(const Exchange::IMessageControl::INotification* remote)
    {
        if (_control != nullptr) {
            _control->UnregisterOutputNotification(remote);
        }
    }

    bool MessageControl::Attach(PluginHost::Channel& channel)
    {
        TRACE(Trace::Information, (Core::Format(_T("Activating channel ID [%d]"), channel.Id()).c_str()));
        return _webSocketExporter->Activate(channel);
    }

    void MessageControl::Detach(PluginHost::Channel& channel)
    {
        TRACE(Trace::Information, (Core::Format(_T("Deactivating channel ID [%d]"), channel.Id()).c_str()));
        _webSocketExporter->Deactivate(channel);
    }

    Core::ProxyType<Core::JSON::IElement> MessageControl::Inbound(const string&)
    {
        return (Core::ProxyType<Core::JSON::IElement>(jsonExportCommandFactory.Element()));
    }

    Core::ProxyType<Core::JSON::IElement> MessageControl::Inbound(const uint32_t ID, const Core::ProxyType<Core::JSON::IElement>& element)
    {
        return (Core::ProxyType<Core::JSON::IElement>(_webSocketExporter->HandleExportCommand(ID, element)));
    }

} // namespace Plugin
} // namespace WPEFramework
