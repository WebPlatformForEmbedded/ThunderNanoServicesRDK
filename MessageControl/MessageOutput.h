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
namespace WPEFramework {
namespace Publishers {

    class Text {
    public:
        Text() = delete;
        Text(const Text&) = delete;
        Text& operator=(const Text&) = delete;

        explicit Text(const bool abbreviated)
            : _abbreviated(abbreviated) {
        }
        ~Text() = default;

    public:
        string Convert(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message);

    private:
        bool _abbreviated;
    };

    class ConsoleOutput : public Messaging::IMessageOutput {
    public:
        ConsoleOutput() = delete;
        ConsoleOutput(const ConsoleOutput&) = delete;
        ConsoleOutput& operator=(const ConsoleOutput&) = delete;

        explicit ConsoleOutput(const bool abbreviate)
            : _convertor(abbreviate) {
        }
        ~ConsoleOutput() override = default;

    public:
        void Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message) override;

    private:
        Text _convertor;
    };

    class SyslogOutput : public Messaging::IMessageOutput {
    public:
        SyslogOutput() = delete;
        SyslogOutput(const SyslogOutput&) = delete;
        SyslogOutput& operator=(const SyslogOutput&) = delete;

        explicit SyslogOutput(const bool abbreviate)
            : _convertor(abbreviate) {
        }
        ~SyslogOutput() override = default;

    public:
        void Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message) override;

    private:
        Text _convertor;
    };
  
    class FileOutput : public Messaging::IMessageOutput {
    public:
        FileOutput() = delete;
        FileOutput(const FileOutput&) = delete;
        FileOutput& operator=(const FileOutput&) = delete;

        explicit FileOutput(const bool abbreviate, const string& filepath)
            : _convertor(abbreviate)
            , _file(filepath) {
            _file.Create();

            if (!_file.IsOpen()) {
                TRACE(Trace::Error, (_T("Could not open file <%s>. Outputting warnings to file unavailable."), filepath));
            }
        }
        ~FileOutput() override {
            if (_file.IsOpen()) {
                _file.Close();
            }
        }

    public:
        void Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message) override;

    private:
        Core::File _file;
        Text _convertor;

    };

    class JSON  {
    private:
        enum class ExtraOutputOptions {
            ABREVIATED    = 0x00,
            FILENAME      = 0x01,
            LINENUMBER    = 0x03, // selecting LINENUMBER will automatically select FILENAME
            MODULE        = 0x04,
            CATEGORY      = 0x08,
            INCLUDINGDATE = 0x10,
            ALL           = 0x1F,
            PAUSED        = 0x20
        };

    public:
        class Data : public Core::JSON::Container {
        public:
            Data(const Data&) = delete;
            Data& operator=(const Data&) = delete;

            Data()
                : Core::JSON::Container()
                , Time()
                , Filename()
                , Linenumber()
                , Category()
                , Module()
                , Message()
            {
                Add(_T("time"), &Time);
                Add(_T("filename"), &Filename);
                Add(_T("linenumber"), &Linenumber);
                Add(_T("category"), &Category);
                Add(_T("module"), &Module);
                Add(_T("message"), &Message);
            }
            ~Data() override = default;

        public:
            Core::JSON::String Time;
            Core::JSON::String Filename;
            Core::JSON::DecUInt32 Linenumber;
            Core::JSON::String Category;
            Core::JSON::String Module;
            Core::JSON::String Message;
        };

    public:
        JSON(const JSON&) = delete;
        JSON& operator=(const JSON&) = delete;

        JSON()
            : _outputoptions(ExtraOutputOptions::ALL) {
        }

        ~JSON() = default;

    public:
        bool FileName() const {
            return ((AsNumber<ExtraOutputOptions>(_outputoptions) & AsNumber(ExtraOutputOptions::FILENAME)) != 0);
        }
        void FileName(const bool enabled) {
            if (enabled == true) {
                _outputoptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputoptions) | AsNumber(ExtraOutputOptions::FILENAME));
            }
            else {
                _outputoptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputoptions) & ~AsNumber(ExtraOutputOptions::FILENAME));
            }
        }
        bool LineNumber() const {
            return ((AsNumber<ExtraOutputOptions>(_outputoptions) & AsNumber(ExtraOutputOptions::LINENUMBER)) != 0);
        }
        void LineNumber(const bool enabled) {
            if (enabled == true) {
                _outputoptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputoptions) | AsNumber(ExtraOutputOptions::LINENUMBER));
            }
            else {
                _outputoptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputoptions) & ~AsNumber(ExtraOutputOptions::LINENUMBER));
            }
        }
        bool Module() const {
            return ((AsNumber<ExtraOutputOptions>(_outputoptions) & AsNumber(ExtraOutputOptions::MODULE)) != 0);
        }
        void Module(const bool enabled) {
            if (enabled == true) {
                _outputoptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputoptions) | AsNumber(ExtraOutputOptions::MODULE));
            }
            else {
                _outputoptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputoptions) & ~AsNumber(ExtraOutputOptions::MODULE));
            }
        }
        bool Category() const {
            return ((AsNumber<ExtraOutputOptions>(_outputoptions) & AsNumber(ExtraOutputOptions::CATEGORY)) != 0);
        }
        void Category(const bool enabled) {
            if (enabled == true) {
                _outputoptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputoptions) | AsNumber(ExtraOutputOptions::CATEGORY));
            }
            else {
                _outputoptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputoptions) & ~AsNumber(ExtraOutputOptions::CATEGORY));
            }
        }
        bool Date() const {
            return ((AsNumber<ExtraOutputOptions>(_outputoptions) & AsNumber(ExtraOutputOptions::INCLUDINGDATE)) != 0);
        }
        void Date(const bool enabled) {
            if (enabled == true) {
                _outputoptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputoptions) | AsNumber(ExtraOutputOptions::INCLUDINGDATE));
            }
            else {
                _outputoptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputoptions) & ~AsNumber(ExtraOutputOptions::INCLUDINGDATE));
            }
        }
        bool Paused() const {
            return ((AsNumber<ExtraOutputOptions>(_outputoptions) & AsNumber(ExtraOutputOptions::PAUSED)) != 0);
        }
        void Paused(const bool enabled) {
            if (enabled == true) {
                _outputoptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputoptions) | AsNumber(ExtraOutputOptions::PAUSED));
            }
            else {
                _outputoptions = static_cast<ExtraOutputOptions>(AsNumber<ExtraOutputOptions>(_outputoptions) & ~AsNumber(ExtraOutputOptions::PAUSED));
            }
        }

        void Convert(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message, Data& response);

    private:
        template <typename E>
        static inline auto AsNumber(E t) -> typename std::underlying_type<E>::type
        {
            return static_cast<typename std::underlying_type<E>::type>(t);
        }

    private:
        std::atomic<ExtraOutputOptions> _outputoptions;
    };

    class UDPOutput : public Messaging::IMessageOutput {
    private:
        class Channel : public Core::SocketDatagram {
        public:
            Channel() = delete;
            Channel(const Channel&) = delete;
            Channel& operator=(const Channel&) = delete;

            explicit Channel(const Core::NodeId& nodeId);
            ~Channel() override;

            void Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message);

        private:
            uint16_t SendData(uint8_t* dataFrame, const uint16_t maxSendSize) override;
            // Unused
            uint16_t ReceiveData(uint8_t*, const uint16_t) override;
            void StateChange() override;

            uint8_t _sendBuffer[Core::Messaging::MessageUnit::DataSize];
            uint16_t _loaded;
            Core::CriticalSection _adminLock;
        };

    public:
        UDPOutput() = delete;
        UDPOutput(const UDPOutput&) = delete;
        UDPOutput& operator=(const UDPOutput&) = delete;

        explicit UDPOutput(const Core::NodeId& nodeId);
        ~UDPOutput() = default;

        void Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message) override;

    private:
        Channel _output;
    };

    class WebSocketOutput : public Messaging::IMessageOutput {
    private:

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
        void Initialize(PluginHost::IShell* service, const uint32_t maxConnections = DefaultMaxConnections) {
            _lock.Lock();
            _server = service;
            _server->AddRef();
            _maxExportConnections = maxConnections;
            _lock.Unlock();
        }
        void Deinitialize() {
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
printf (" ----- %s ------ %d -------\n", __FUNCTION__, __LINE__);
            _lock.Lock();
printf (" ----- %s ------ %d -------\n", __FUNCTION__, __LINE__);

            if (_channels.size() < _maxExportConnections) {
                ChannelMap::iterator index = _channels.find(id);

                ASSERT(index == _channels.end());

                _channels.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple());

                accepted = true;
            }

printf (" ----- %s ------ %d -------\n", __FUNCTION__, __LINE__);
            _lock.Unlock();
printf (" ----- %s ------ %d -------\n", __FUNCTION__, __LINE__);

            return accepted;
        }

        bool Detach(const uint32_t id) {
            bool deactivated = false;

            _lock.Lock();

            ChannelMap::iterator index = _channels.find(id);

            if (index != _channels.end()) {
                _channels.erase(index);
                deactivated = true;
            }

            _lock.Unlock();

            return deactivated;
        }

        uint32_t MaxConnections() const {
            return (_maxExportConnections);
        }

        Core::ProxyType<Core::JSON::IElement> Received (const uint32_t id, const Core::ProxyType<Core::JSON::IElement>& element) {
            Core::ProxyType<ExportCommand> info = Core::ProxyType<ExportCommand>(element);

            if (info.IsValid() == false) {
                element.Release();
            }
            else {
                _lock.Lock();

                ChannelMap::iterator index = _channels.find(id);

                if (index != _channels.end()) {
                    if (info->Filename.IsSet() == true) {
                        index->second.FileName(info->Filename == true);
                    }
                    if (info->Identifier.IsSet() == true) {
                        index->second.LineNumber(info->Identifier == true);
                    }
                    if (info->Category.IsSet() == true) {
                        index->second.Category(info->Category == true);
                    }
                    if (info->IncludingDate.IsSet() == true) {
                        index->second.Date(info->IncludingDate == true);
                    }
                    if (info->Paused.IsSet() == true) {
                        index->second.Paused(info->Paused == true);
                    }

                    info->Clear();
                    info->Filename = index->second.FileName();
                    info->Identifier = index->second.LineNumber();
                    info->Category = index->second.Category();
                    info->IncludingDate = index->second.Date();
                    info->Paused = index->second.Paused();
                }

                _lock.Unlock();
            }

            return (element);
        }

        void Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message) override {

            std::list<std::pair<uint32_t, Core::ProxyType<Core::JSON::IElement>>> cachedList;
            PluginHost::IShell* server = nullptr;

            _lock.Lock();

            if (_server != nullptr) {

                for (auto& item : _channels) {
                    if (item.second.Paused() == false) {
                        Core::ProxyType<JSON::Data> data = _jsonExportDataFactory.Element();
                        item.second.Convert(info, message, *data);
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

}
}
