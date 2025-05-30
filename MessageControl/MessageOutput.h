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

#pragma once
#include "Module.h"

namespace Thunder {

namespace Publishers {

    struct IPublish {
        virtual ~IPublish() = default;

        virtual void Message(const Core::Messaging::MessageInfo& metadata, const string& text) = 0;
    };

    class Text {
    public:
        Text() = delete;
        Text(const Text&) = delete;
        Text& operator=(const Text&) = delete;

        explicit Text(const Core::Messaging::MessageInfo::abbreviate abbreviated)
            : _abbreviated(abbreviated)
        {
        }
        ~Text() = default;

    public:
        string Convert(const Core::Messaging::MessageInfo& metadata, const string& text);

    private:
        Core::Messaging::MessageInfo::abbreviate _abbreviated;
    };

    class ConsoleOutput : public IPublish {
    public:
        ConsoleOutput() = delete;
        ConsoleOutput(const ConsoleOutput&) = delete;
        ConsoleOutput& operator=(const ConsoleOutput&) = delete;

        explicit ConsoleOutput(const Core::Messaging::MessageInfo::abbreviate abbreviate)
            : _convertor(abbreviate)
        {
        }
        ~ConsoleOutput() override = default;

    public:
        void Message(const Core::Messaging::MessageInfo& metadata, const string& text) override;

    private:
        Text _convertor;
    };

    class SyslogOutput : public IPublish {
    public:
        SyslogOutput() = delete;
        SyslogOutput(const SyslogOutput&) = delete;
        SyslogOutput& operator=(const SyslogOutput&) = delete;

        explicit SyslogOutput(const Core::Messaging::MessageInfo::abbreviate abbreviate)
            : _convertor(abbreviate)
        {
        }
        ~SyslogOutput() override = default;

    public:
        void Message(const Core::Messaging::MessageInfo& metadata, const string& text) override;

    private:
        Text _convertor;
    };
  
    class FileOutput : public IPublish {
    public:
        FileOutput() = delete;
        FileOutput(const FileOutput&) = delete;
        FileOutput& operator=(const FileOutput&) = delete;

        explicit FileOutput(const Core::Messaging::MessageInfo::abbreviate abbreviate, const string& filepath)
            : _convertor(abbreviate)
            , _file(filepath)
        {
            _file.Create();

            if (!_file.IsOpen()) {
                TRACE(Trace::Error, (_T("Could not open file <%s>. Outputting warnings to file unavailable."), filepath));
            }
        }
        ~FileOutput() override
        {
            if (_file.IsOpen()) {
                _file.Close();
            }
        }

    public:
        void Message(const Core::Messaging::MessageInfo& metadata, const string& text) override;

    private:
        Text _convertor;
        Core::File _file;
    };

    class JSON  {
    private:
        enum class ExtraOutputOptions {
            ABREVIATED    = 0x000,
            FILENAME      = 0x001,
            LINENUMBER    = 0x003, // selecting LINENUMBER will automatically select FILENAME
            CLASSNAME     = 0x004,
            MODULE        = 0x008,
            CATEGORY      = 0x010,
            CALLSIGN      = 0x020,
            INCLUDINGDATE = 0x040,
            PROCESSID     = 0x080,
            PROCESSNAME   = 0x100,
            ALL           = 0x7FF,
            PAUSED        = 0x800
        };

    public:
        class Data : public Core::JSON::Container {
        public:
            Data(const Data&) = delete;
            Data& operator=(const Data&) = delete;

            Data()
                : Core::JSON::Container()
                , Time()
                , FileName()
                , LineNumber()
                , ClassName()
                , Category()
                , Module()
                , Callsign()
                , ProcessId()
                , ProcessName()
                , Message()
            {
                Add(_T("time"), &Time);
                Add(_T("filename"), &FileName);
                Add(_T("linenumber"), &LineNumber);
                Add(_T("classname"), &ClassName);
                Add(_T("category"), &Category);
                Add(_T("module"), &Module);
                Add(_T("callsign"), &Callsign);
                Add(_T("processid"), &ProcessId);
                Add(_T("processname"), &ProcessName);
                Add(_T("message"), &Message);
            }
            ~Data() override = default;

        public:
            Core::JSON::String Time;
            Core::JSON::String FileName;
            Core::JSON::DecUInt32 LineNumber;
            Core::JSON::String ClassName;
            Core::JSON::String Category;
            Core::JSON::String Module;
            Core::JSON::String Callsign;
            Core::JSON::DecUInt32 ProcessId;
            Core::JSON::String ProcessName;
            Core::JSON::String Message;
        };

    public:
        JSON(const JSON&) = delete;
        JSON& operator=(const JSON&) = delete;

        JSON()
            : _outputOptions(ExtraOutputOptions::ALL)
        {
        }
        ~JSON() = default;

    public:
        bool FileName() const {
            return ((AsNumber<ExtraOutputOptions>(_outputOptions) & AsNumber(ExtraOutputOptions::FILENAME)) != 0);
        }

        void FileName(const bool enabled)
        {
            if (enabled == true) {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) | AsNumber(ExtraOutputOptions::FILENAME));
            }
            else {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) & ~AsNumber(ExtraOutputOptions::FILENAME));
            }
        }

        bool LineNumber() const {
            return ((AsNumber<ExtraOutputOptions>(_outputOptions) & AsNumber(ExtraOutputOptions::LINENUMBER)) != 0);
        }

        void LineNumber(const bool enabled)
        {
            if (enabled == true) {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) | AsNumber(ExtraOutputOptions::LINENUMBER));
            }
            else {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) & ~AsNumber(ExtraOutputOptions::LINENUMBER));
            }
        }

        bool ClassName() const {
            return ((AsNumber<ExtraOutputOptions>(_outputOptions) & AsNumber(ExtraOutputOptions::CLASSNAME)) != 0);
        }

        void ClassName(const bool enabled)
        {
            if (enabled == true) {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) | AsNumber(ExtraOutputOptions::CLASSNAME));
            }
            else {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) & ~AsNumber(ExtraOutputOptions::CLASSNAME));
            }
        }

        bool Module() const {
            return ((AsNumber<ExtraOutputOptions>(_outputOptions) & AsNumber(ExtraOutputOptions::MODULE)) != 0);
        }

        void Module(const bool enabled)
        {
            if (enabled == true) {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) | AsNumber(ExtraOutputOptions::MODULE));
            }
            else {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) & ~AsNumber(ExtraOutputOptions::MODULE));
            }
        }

        bool Category() const {
            return ((AsNumber<ExtraOutputOptions>(_outputOptions) & AsNumber(ExtraOutputOptions::CATEGORY)) != 0);
        }

        void Category(const bool enabled)
        {
            if (enabled == true) {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) | AsNumber(ExtraOutputOptions::CATEGORY));
            }
            else {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) & ~AsNumber(ExtraOutputOptions::CATEGORY));
            }
        }

        bool Callsign() const {
            return ((AsNumber<ExtraOutputOptions>(_outputOptions) & AsNumber(ExtraOutputOptions::CALLSIGN)) != 0);
        }

        void Callsign(const bool enabled)
        {
            if (enabled == true) {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) | AsNumber(ExtraOutputOptions::CALLSIGN));
            }
            else {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) & ~AsNumber(ExtraOutputOptions::CALLSIGN));
            }
        }

        bool ProcessId() const {
            return ((AsNumber<ExtraOutputOptions>(_outputOptions) & AsNumber(ExtraOutputOptions::PROCESSID)) != 0);
        }

        void ProcessId(const bool enabled)
        {
            if (enabled == true) {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) | AsNumber(ExtraOutputOptions::PROCESSID));
            }
            else {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) & ~AsNumber(ExtraOutputOptions::PROCESSID));
            }
        }

        bool ProcessName() const {
            return ((AsNumber<ExtraOutputOptions>(_outputOptions) & AsNumber(ExtraOutputOptions::PROCESSNAME)) != 0);
        }

        void ProcessName(const bool enabled)
        {
            if (enabled == true) {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) | AsNumber(ExtraOutputOptions::PROCESSNAME));
            }
            else {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) & ~AsNumber(ExtraOutputOptions::PROCESSNAME));
            }
        }

        bool Date() const {
            return ((AsNumber<ExtraOutputOptions>(_outputOptions) & AsNumber(ExtraOutputOptions::INCLUDINGDATE)) != 0);
        }

        void Date(const bool enabled)
        {
            if (enabled == true) {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) | AsNumber(ExtraOutputOptions::INCLUDINGDATE));
            }
            else {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) & ~AsNumber(ExtraOutputOptions::INCLUDINGDATE));
            }
        }

        bool Paused() const {
            return ((AsNumber<ExtraOutputOptions>(_outputOptions) & AsNumber(ExtraOutputOptions::PAUSED)) != 0);
        }

        void Paused(const bool enabled)
        {
            if (enabled == true) {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) | AsNumber(ExtraOutputOptions::PAUSED));
            }
            else {
                _outputOptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputOptions) & ~AsNumber(ExtraOutputOptions::PAUSED));
            }
        }

        void Convert(const Core::Messaging::MessageInfo& metadata, const string& text, Data& info);

    private:
        template <typename E>
        static inline auto AsNumber(E t) -> typename std::underlying_type<E>::type {
            return (static_cast<typename std::underlying_type<E>::type>(t));
        }

    private:
        std::atomic<ExtraOutputOptions> _outputOptions;
    };

    class UDPOutput : public IPublish {
    private:
        static constexpr uint16_t UDPBufferSize = 4 * 1024;

        class Channel : public Core::SocketDatagram {
        public:
            Channel() = delete;
            Channel(const Channel&) = delete;
            Channel& operator=(const Channel&) = delete;

            explicit Channel(const Core::NodeId& nodeId);
            ~Channel() override;

            void Output(const string& text);

        private:
            uint16_t SendData(uint8_t* dataFrame, const uint16_t maxSendSize) override;
            // Unused
            uint16_t ReceiveData(uint8_t*, const uint16_t) override;
            void StateChange() override;

            uint8_t _sendBuffer[UDPBufferSize];
            uint16_t _loaded;
            Core::CriticalSection _adminLock;
        };

        class Notification : public PluginHost::ISubSystem::INotification {
        public:
            Notification() = delete;
            Notification(const Notification&) = delete;
            Notification(Notification&&) = delete;
            Notification& operator=(const Notification&) = delete;
            Notification& operator=(Notification&&) = delete;

            explicit Notification(UDPOutput& parent)
                : _parent(parent) {
            }
            ~Notification() = default;

        public:
            void Updated() override
            {
                _parent.UpdateChannel();
            }

        BEGIN_INTERFACE_MAP(Notification)
            INTERFACE_ENTRY(PluginHost::ISubSystem::INotification)
        END_INTERFACE_MAP

        private:
            UDPOutput& _parent;
        };

    public:
        UDPOutput() = delete;
        UDPOutput(const UDPOutput&) = delete;
        UDPOutput& operator=(const UDPOutput&) = delete;

        explicit UDPOutput(const Core::Messaging::MessageInfo::abbreviate abbreviate, const Core::NodeId& nodeId, PluginHost::IShell* service, const string& interface);

        ~UDPOutput() override
        {
            if (_subSystem != nullptr) {
                _subSystem->Unregister(&_notification);
                _subSystem->Release();
                _subSystem = nullptr;
            }
        }

        void UpdateChannel();
        void Message(const Core::Messaging::MessageInfo& metadata, const string& text) override;

    private:
        Text _convertor;
        Channel _output;
        Core::SinkType<Notification> _notification;
        PluginHost::ISubSystem* _subSystem;
        string _interface;
    };

    class WebSocketOutput : public IPublish {
    private:
        class ExportCommand : public Core::JSON::Container {
        public:
            ExportCommand(const ExportCommand&) = delete;
            ExportCommand& operator=(const ExportCommand&) = delete;

            ExportCommand()
                : Core::JSON::Container()
                , FileName()
                , LineNumber()
                , Category()
                , Module()
                , Callsign()
                , ProcessId()
                , ProcessName()
                , IncludingDate()
                , Paused()
            {
                Add(_T("filename"), &FileName);
                Add(_T("linenumber"), &LineNumber);
                Add(_T("classname"), &ClassName);
                Add(_T("category"), &Category);
                Add(_T("module"), &Module);
                Add(_T("callsign"), &Callsign);
                Add(_T("processid"), &ProcessId);
                Add(_T("processname"), &ProcessName);
                Add(_T("includingdate"), &IncludingDate);
                Add(_T("paused"), &Paused);
            }
            ~ExportCommand() override = default;

        public:
            Core::JSON::Boolean FileName;
            Core::JSON::Boolean LineNumber;
            Core::JSON::Boolean ClassName;
            Core::JSON::Boolean Category;
            Core::JSON::Boolean Module;
            Core::JSON::Boolean Callsign;
            Core::JSON::Boolean ProcessId;
            Core::JSON::Boolean ProcessName;
            Core::JSON::Boolean IncludingDate;
            Core::JSON::Boolean Paused;
        };

        using ChannelMap = std::unordered_map<uint32_t, JSON>;

    public:
        static constexpr uint16_t DefaultMaxConnections = 5;

    public:
        WebSocketOutput(const WebSocketOutput& copy) = delete;
        WebSocketOutput& operator=(const WebSocketOutput&) = delete;

        explicit WebSocketOutput()
            : _lock()
            , _server(nullptr)
            , _channels()
            , _maxExportConnections(0)
            , _jsonExportDataFactory(2)
            , _jsonExportCommandFactory(2)
        {
        }
        ~WebSocketOutput() override = default;

    public:
        void Initialize(PluginHost::IShell* service, const uint32_t maxConnections = DefaultMaxConnections)
        {
            _lock.Lock();

            _server = service;
            _server->AddRef();
            _maxExportConnections = maxConnections;

            _lock.Unlock();
        }

        void Deinitialize()
        {
            _lock.Lock();

            _server->Release();
            _server = nullptr;
            _channels.clear();
            _maxExportConnections = 0;

            _lock.Unlock();
        }

        bool Attach(const uint32_t id)
        {
            bool accepted = false;
            _lock.Lock();

            if (_channels.size() < _maxExportConnections) {

                ASSERT(_channels.find(id) == _channels.end());

                _channels.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple());

                accepted = true;
            }

            _lock.Unlock();

            return (accepted);
        }

        bool Detach(const uint32_t id)
        {
            bool deactivated = false;

            _lock.Lock();

            ChannelMap::iterator index = _channels.find(id);

            if (index != _channels.end()) {
                _channels.erase(index);
                deactivated = true;
            }

            _lock.Unlock();

            return (deactivated);
        }

        uint32_t MaxConnections() const {
            return (_maxExportConnections);
        }

        Core::ProxyType<Core::JSON::IElement> Received(const uint32_t id, const Core::ProxyType<Core::JSON::IElement>& element)
        {
            Core::ProxyType<ExportCommand> info = Core::ProxyType<ExportCommand>(element);

            if (info.IsValid() == false) {
                element.Release();
            }
            else {
                _lock.Lock();

                ChannelMap::iterator index = _channels.find(id);

                if (index != _channels.end()) {
                    if (info->FileName.IsSet() == true) {
                        index->second.FileName(info->FileName == true);
                    }
                    if (info->LineNumber.IsSet() == true) {
                        index->second.LineNumber(info->LineNumber == true);
                    }
                    if (info->ClassName.IsSet() == true) {
                        index->second.ClassName(info->ClassName == true);
                    }
                    if (info->Category.IsSet() == true) {
                        index->second.Category(info->Category == true);
                    }
                    if (info->Module.IsSet() == true) {
                        index->second.Module(info->Module == true);
                    }
                    if (info->Callsign.IsSet() == true) {
                        index->second.Callsign(info->Callsign == true);
                    }
                    if (info->ProcessId.IsSet() == true) {
                        index->second.ProcessId(info->ProcessId == true);
                    }
                    if (info->ProcessName.IsSet() == true) {
                        index->second.ProcessName(info->ProcessName == true);
                    }
                    if (info->IncludingDate.IsSet() == true) {
                        index->second.Date(info->IncludingDate == true);
                    }
                    if (info->Paused.IsSet() == true) {
                        index->second.Paused(info->Paused == true);
                    }

                    info->Clear();
                    info->FileName = index->second.FileName();
                    info->LineNumber = index->second.LineNumber();
                    info->ClassName = index->second.ClassName();
                    info->Category = index->second.Category();
                    info->Module = index->second.Module();
                    info->Callsign = index->second.Callsign();
                    info->ProcessId = index->second.ProcessId();
                    info->ProcessName = index->second.ProcessName();
                    info->IncludingDate = index->second.Date();
                    info->Paused = index->second.Paused();
                }

                _lock.Unlock();
            }

            return (element);
        }

        void Message(const Core::Messaging::MessageInfo& metadata, const string& text) override
        {
            std::list<std::pair<uint32_t, Core::ProxyType<Core::JSON::IElement>>> cachedList;
            PluginHost::IShell* server = nullptr;

            _lock.Lock();

            if (_server != nullptr) {
                for (auto& item : _channels) {
                    if (item.second.Paused() == false) {
                        Core::ProxyType<JSON::Data> data = _jsonExportDataFactory.Element();
                        item.second.Convert(metadata, text, *data);
                        cachedList.emplace_back(item.first, Core::ProxyType<Core::JSON::IElement>(data));
                    }
                }
                if (cachedList.empty() == false) {
                    server = _server;
                    server->AddRef();
                }
            }

            _lock.Unlock();

            if (server != nullptr) {
                for (std::pair<uint32_t, Core::ProxyType<Core::JSON::IElement>>& entry : cachedList) {
                    _server->Submit(entry.first, entry.second);
                }
                cachedList.clear();
                server->Release();
            }
        }

        Core::ProxyType<Core::JSON::IElement> Command() {
            return (Core::ProxyType<Core::JSON::IElement>(_jsonExportCommandFactory.Element()));
        }

    private:
        mutable Core::CriticalSection _lock;
        PluginHost::IShell* _server;
        ChannelMap _channels;
        uint32_t _maxExportConnections;
        Core::ProxyPoolType<JSON::Data> _jsonExportDataFactory;
        Core::ProxyPoolType<ExportCommand> _jsonExportCommandFactory;
    };

} // namespace Publishers
}
