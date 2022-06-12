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

#include "WarningReportingControl.h"
#include "WarningOutput.h"

#include <algorithm>

namespace WPEFramework {

ENUM_CONVERSION_BEGIN(Plugin::WarningReportingControl::state)

    { Plugin::WarningReportingControl::state::ENABLED, _TXT("enabled") },
    { Plugin::WarningReportingControl::state::DISABLED, _TXT("disabled") },
    { Plugin::WarningReportingControl::state::TRISTATED, _TXT("tristated") },

    ENUM_CONVERSION_END(Plugin::WarningReportingControl::state);

namespace Plugin {

    namespace {

        static Metadata<WarningReportingControl> metadata(
            // Version
            1, 0, 0,
            // Preconditions
            {},
            // Terminations
            {},
            // Controls
            {} 
        );
    }

    Core::ProxyPoolType<Plugin::WarningReportingJSONOutput::Data> jsonExportDataFactory(2);
    constexpr uint16_t MAX_CONNECTIONS = 5;

    class WebSocketExporter : public WarningReporting::IWarningReportingMedia {

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
        class WarningChannelOutput : public Plugin::WarningReportingJSONOutput {

        public:
            WarningChannelOutput(const WarningChannelOutput&) = delete;
            WarningChannelOutput& operator=(const WarningChannelOutput&) = delete;

            explicit WarningChannelOutput(PluginHost::Channel& channel)
                : Plugin::WarningReportingJSONOutput(channel)
            {
            }
            ~WarningChannelOutput() override = default;

            Core::ProxyType<Core::JSON::IElement> Process(const Core::ProxyType<Core::JSON::IElement>& element);
            Core::ProxyType<Data> GetDataContainer() override
            {
                return jsonExportDataFactory.Element();
            }
        };

    public:
        WebSocketExporter(const WebSocketExporter& copy) = delete;
        WebSocketExporter& operator=(const WebSocketExporter&) = delete;

        WebSocketExporter(const uint32_t maxConnections = MAX_CONNECTIONS)
            : _warningChannelOutputs()
            , _lock()
            , _maxExportConnections(maxConnections)
        {
        }

        ~WebSocketExporter() override = default;

        bool Activate(PluginHost::Channel& channel)
        {

            bool accepted = false;

            _lock.Lock();

            if ((_maxExportConnections != 0) && (_maxExportConnections - _warningChannelOutputs.size() > 0)) {
                ASSERT(0 == _warningChannelOutputs.count(channel.Id()));
                _warningChannelOutputs.emplace(std::make_pair(channel.Id(), new WarningChannelOutput(channel)));
                accepted = true;
            }

            _lock.Unlock();

            return accepted;
        }

        bool Deactivate(PluginHost::Channel& channel)
        {

            bool deactivated = false;

            _lock.Lock();

            if (_warningChannelOutputs.count(channel.Id() != 0)) {
                _warningChannelOutputs.erase(channel.Id());
                deactivated = true;
            }

            _lock.Unlock();

            return deactivated;
        }

        Core::ProxyType<Core::JSON::IElement> HandleExportCommand(const uint32_t ID, const Core::ProxyType<Core::JSON::IElement>& element);
        void Output(const char fileName[], const uint32_t lineNumber, const char identifer[], const WarningReporting::IWarningEvent* information) override
        {

            _lock.Lock();

            for (auto& item : _warningChannelOutputs) {
                item.second->Output(fileName, lineNumber, identifer, information);
            }

            _lock.Unlock();
        }

    private:
        std::unordered_map<uint32_t, std::unique_ptr<Plugin::WarningReportingJSONOutput>> _warningChannelOutputs;
        mutable Core::CriticalSection _lock;
        const uint32_t _maxExportConnections;
    };

    static Core::ProxyPoolType<WebSocketExporter::ExportCommand> jsonExportCommandFactory(2);

    Core::ProxyType<Core::JSON::IElement> WebSocketExporter::HandleExportCommand(const uint32_t ID,
        const Core::ProxyType<Core::JSON::IElement>& element)
    {

        Core::ProxyType<Core::JSON::IElement> response;

        _lock.Lock();

        auto index = _warningChannelOutputs.find(ID);
        if (index != _warningChannelOutputs.end()) {
            response = index->second->Process(element);
        }

        _lock.Unlock();

        return response;
    }

    Core::ProxyType<Core::JSON::IElement> WebSocketExporter::WarningChannelOutput::Process(const Core::ProxyType<Core::JSON::IElement>& element)
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
                options = static_cast<ExtraOutputOptions>(AsNumber(options) | AsNumber(ExtraOutputOptions::IDENTIFIER));
            } else {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) & ~AsNumber(ExtraOutputOptions::IDENTIFIER));
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
        response->Identifier = ((AsNumber(options) & AsNumber(ExtraOutputOptions::IDENTIFIER)) != 0);
        response->Category = ((AsNumber(options) & AsNumber(ExtraOutputOptions::CATEGORY)) != 0);
        response->IncludingDate = ((AsNumber(options) & AsNumber(ExtraOutputOptions::INCLUDINGDATE)) != 0);
        response->Paused = IsPaused();

        return (Core::ProxyType<Core::JSON::IElement>(response));
    }

    /* static */ string WarningReportingControl::Observer::Source::SourceName(const string& prefix, RPC::IRemoteConnection* connection)
    {
        string pathName;
        pathName = Core::Directory::Normalize(prefix) + WarningReporting::CyclicBufferName;

        if (connection != nullptr) {
            pathName += '.' + Core::NumberType<uint32_t>(connection->Id()).Text();
        }

        return pathName;
    }

    const string WarningReportingControl::Initialize(PluginHost::IShell* service)
    {
        ASSERT(_service == nullptr);
        ASSERT(_outputs.size() == 0);

        _service = service;
        _config.FromString(_service->ConfigLine());
        _warningsPath = service->VolatilePath();
        _outputOnlyWarnings = _config.WarningsOnly.Value();

        //warning buffers are placed in volatile path base (eg. /tmp/, not /tmp/WarnigReportingControl)
        std::size_t pos = service->Callsign().length();
        if ((pos = _warningsPath.find_last_of('/', (_warningsPath.length() >= pos ? _warningsPath.length() - pos : string::npos))) != string::npos) {
            _warningsPath = _warningsPath.substr(0, pos);
        }

        //always output somewhere
        if (((service->Background() == false) && (_config.Console.IsSet() == false) && (_config.SysLog.IsSet() == false))
            || ((_config.Console.IsSet() == true) && (_config.Console.Value() == true))) {
            _outputs.emplace_back(new WarningReportConsoleOutput(_config.Abbreviated.Value()));
        }
        if (((service->Background() == true) && (_config.Console.IsSet() == false) && (_config.SysLog.IsSet() == false))
            || ((_config.SysLog.IsSet() == true) && (_config.SysLog.Value() == true))) {
            _outputs.emplace_back(new WarningReportSyslogOutput(_config.Abbreviated.Value()));
        }
        if (_config.FileName.IsSet()) {
            string fullPath = service->VolatilePath() + _config.FileName.Value();
            _outputs.emplace_back(new WarningReportFileOutput(_config.Abbreviated.Value(), fullPath));
        }

        uint16_t maxExportConnections = _config.MaxExportConnections.IsSet() ? _config.MaxExportConnections.Value() : MAX_CONNECTIONS;
        if (maxExportConnections > 0) {
            _webSocketExporterInstance = new WebSocketExporter(maxExportConnections);
            _outputs.emplace_back(_webSocketExporterInstance);
        }

        _service->Register(&_observer);
        _observer.Start();

        return _T("");
    }

    void WarningReportingControl::Deinitialize(PluginHost::IShell* service VARIABLE_IS_NOT_USED)
    {
        ASSERT(service == _service);

        _service->Unregister(&_observer);

        // Stop observing..
        _observer.Stop();

        _outputs.clear();
    }

    bool WarningReportingControl::Attach(PluginHost::Channel& channel)
    {
        TRACE(Trace::Information, (Core::Format(_T("Activating channel ID [%d]"), channel.Id()).c_str()));
        return _webSocketExporterInstance->Activate(channel);
    }

    void WarningReportingControl::Detach(PluginHost::Channel& channel)
    {
        TRACE(Trace::Information, (Core::Format(_T("Deactivating channel ID [%d]"), channel.Id()).c_str()));
        _webSocketExporterInstance->Deactivate(channel);
    }

    Core::ProxyType<Core::JSON::IElement> WarningReportingControl::Inbound(const string&)
    {
        return (Core::ProxyType<Core::JSON::IElement>(jsonExportCommandFactory.Element()));
    }

    Core::ProxyType<Core::JSON::IElement> WarningReportingControl::Inbound(const uint32_t ID, const Core::ProxyType<Core::JSON::IElement>& element)
    {
        return (Core::ProxyType<Core::JSON::IElement>(_webSocketExporterInstance->HandleExportCommand(ID, element)));
    }

    string WarningReportingControl::Information() const
    {
        // No additional info to report.
        return nullptr;
    }

    void WarningReportingControl::Dispatch(const WarningInformation& information)
    {
        ASSERT(information.event != nullptr);
        if (information.event != nullptr) {
            if ((_outputOnlyWarnings && information.event->IsWarning()) || (!_outputOnlyWarnings)) {
                for (const auto& output : _outputs) {
                    output->Output(information.filename.c_str(), information.lineNumber, information.identifier.c_str(), information.event.get());
                }
            }
        }
    }
}
}
