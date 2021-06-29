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

namespace {

WPEFramework::Core::ProxyPoolType<WPEFramework::Plugin::WarningReportingJSONOutput::Data> jsonExportDataFactory(2);
constexpr uint16_t MAX_CONNECTIONS = 5;

class WebSocketExporter : public WPEFramework::WarningReporting::IWarningReportingMedia {

public:
    class ExportCommand : public WPEFramework::Core::JSON::Container {

    public:
        ExportCommand(const ExportCommand&) = delete;
        ExportCommand& operator=(const ExportCommand&) = delete;

        ExportCommand()
            : WPEFramework::Core::JSON::Container()
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
        WPEFramework::Core::JSON::Boolean Filename;
        WPEFramework::Core::JSON::Boolean Identifier;
        WPEFramework::Core::JSON::Boolean Category;
        WPEFramework::Core::JSON::Boolean IncludingDate;
        WPEFramework::Core::JSON::Boolean Paused;
    };

public:
    class TraceChannelOutput : public WPEFramework::Plugin::WarningReportingJSONOutput {

    public:
        TraceChannelOutput(const TraceChannelOutput&) = delete;
        TraceChannelOutput& operator=(const TraceChannelOutput&) = delete;

        explicit TraceChannelOutput(WPEFramework::PluginHost::Channel& channel)
            : WPEFramework::Plugin::WarningReportingJSONOutput(channel)
        {
        }
        ~TraceChannelOutput() override = default;

        WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement> Process(const WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement>& element);
        WPEFramework::Core::ProxyType<Data> GetDataContainer() override
        {
            return jsonExportDataFactory.Element();
        }
    };

public:
    WebSocketExporter(const WebSocketExporter& copy) = delete;
    WebSocketExporter& operator=(const WebSocketExporter&) = delete;

    WebSocketExporter(const uint32_t maxConnections = MAX_CONNECTIONS)
        : _traceChannelOutputs()
        , _lock()
        , _maxExportConnections(maxConnections)
    {
    }

    ~WebSocketExporter() override = default;

    bool Activate(WPEFramework::PluginHost::Channel& channel)
    {

        bool accepted = false;

        _lock.Lock();

        if ((_maxExportConnections != 0) && (_maxExportConnections - _traceChannelOutputs.size() > 0)) {
            ASSERT(0 == _traceChannelOutputs.count(channel.Id()));
            _traceChannelOutputs.emplace(std::make_pair(channel.Id(), new TraceChannelOutput(channel)));
            accepted = true;
        }

        _lock.Unlock();

        return accepted;
    }

    bool Deactivate(WPEFramework::PluginHost::Channel& channel)
    {

        bool deactivated = false;

        _lock.Lock();

        if (_traceChannelOutputs.count(channel.Id() != 0)) {
            _traceChannelOutputs.erase(channel.Id());
            deactivated = true;
        }

        _lock.Unlock();

        return deactivated;
    }

    WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement> HandleExportCommand(const uint32_t ID, const WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement>& element);
    void Output(const char fileName[], const uint32_t lineNumber, const char identifer[], const WPEFramework::WarningReporting::IWarningEvent* information) override
    {

        _lock.Lock();

        for (auto& item : _traceChannelOutputs) {
            item.second->Output(fileName, lineNumber, identifer, information);
        }

        _lock.Unlock();
    }

private:
    using TraceChannelOutputPtr = std::unique_ptr<WPEFramework::Plugin::WarningReportingJSONOutput>;

    std::unordered_map<uint32_t, TraceChannelOutputPtr> _traceChannelOutputs;
    mutable WPEFramework::Core::CriticalSection _lock;
    const uint32_t _maxExportConnections;
};

static WPEFramework::Core::ProxyPoolType<WebSocketExporter::ExportCommand> jsonExportCommandFactory(2);

WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement> WebSocketExporter::HandleExportCommand(const uint32_t ID,
    const WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement>& element)
{

    WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement> response;

    _lock.Lock();

    auto index = _traceChannelOutputs.find(ID);
    if (index != _traceChannelOutputs.end()) {
        response = index->second->Process(element);
    }

    _lock.Unlock();

    return response;
}

WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement> WebSocketExporter::TraceChannelOutput::Process(const WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement>& element)
{
    WPEFramework::Core::ProxyType<WebSocketExporter::ExportCommand>
        inbound(WPEFramework::Core::proxy_cast<WebSocketExporter::ExportCommand>(element));

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

    WPEFramework::Core::ProxyType<ExportCommand> response(jsonExportCommandFactory.Element());

    response->Filename = ((AsNumber(options) & AsNumber(ExtraOutputOptions::FILENAME)) != 0);
    response->Identifier = ((AsNumber(options) & AsNumber(ExtraOutputOptions::IDENTIFIER)) != 0);
    response->Category = ((AsNumber(options) & AsNumber(ExtraOutputOptions::CATEGORY)) != 0);
    response->IncludingDate = ((AsNumber(options) & AsNumber(ExtraOutputOptions::INCLUDINGDATE)) != 0);
    response->Paused = IsPaused();

    return WPEFramework::Core::proxy_cast<WPEFramework::Core::JSON::IElement>(response);
}
}

namespace WPEFramework {

ENUM_CONVERSION_BEGIN(Plugin::WarningReportingControl::state)

    { Plugin::WarningReportingControl::state::ENABLED, _TXT("enabled") },
    { Plugin::WarningReportingControl::state::DISABLED, _TXT("disabled") },
    { Plugin::WarningReportingControl::state::TRISTATED, _TXT("tristated") },

    ENUM_CONVERSION_END(Plugin::WarningReportingControl::state);

namespace Plugin {

    SERVICE_REGISTRATION(WarningReportingControl, 1, 0);

    /* static */ string WarningReportingControl::Observer::Source::SourceName(const string& prefix, RPC::IRemoteConnection* connection)
    {
        string pathName;
        pathName = Core::Directory::Normalize(prefix) + WarningReporting::CyclicBufferName;

        if (connection != nullptr) {
            pathName += '.' + Core::NumberType<uint32_t>(connection->Id()).Text();
        }

        return (pathName);
    }

    const string WarningReportingControl::Initialize(PluginHost::IShell* service)
    {
        ASSERT(_service == nullptr);
        ASSERT(_outputs.size() == 0);

        _service = service;
        _config.FromString(_service->ConfigLine());
        _warningsPath = service->VolatilePath();

        std::size_t pos = service->Callsign().length();
        if ((pos = _warningsPath.find_last_of('/', (_warningsPath.length() >= pos ? _warningsPath.length() - pos : string::npos))) != string::npos) {
            _warningsPath = _warningsPath.substr(0, pos);
        }

        if (((service->Background() == false) && (_config.Console.IsSet() == false) && (_config.SysLog.IsSet() == false))
            || ((_config.Console.IsSet() == true) && (_config.Console.Value() == true))) {
            _outputs.emplace(typeid(WarningReportConsoleOutput), new WarningReportConsoleOutput(_config.Abbreviated.Value()));
        }
        if (((service->Background() == true) && (_config.Console.IsSet() == false) && (_config.SysLog.IsSet() == false))
            || ((_config.SysLog.IsSet() == true) && (_config.SysLog.Value() == true))) {
            _outputs.emplace(typeid(WarningReportSyslogOutput), new WarningReportSyslogOutput(_config.Abbreviated.Value()));
        }
        if (_config.FilePath.IsSet()) {
            _outputs.emplace(typeid(WarningReportFileOutput), new WarningReportFileOutput(_config.Abbreviated.Value(), _config.FilePath.Value()));
        }

        uint16_t maxExportConnections = _config.MaxExportConnections.IsSet() ? _config.MaxExportConnections.Value() : MAX_CONNECTIONS;
        if (maxExportConnections > 0) {
            _outputs.emplace(typeid(WebSocketExporter), new WebSocketExporter(maxExportConnections));
        }

        _service->Register(&_observer);
        _observer.Start();

        return (_T(""));
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
        auto index = _outputs.find(typeid(WebSocketExporter));
        if (index != _outputs.end()) {
            auto instance = static_cast<WebSocketExporter*>(index->second.get());
            return instance->Activate(channel);
        }
    }

    void WarningReportingControl::Detach(PluginHost::Channel& channel)
    {
        TRACE(Trace::Information, (Core::Format(_T("Deactivating channel ID [%d]"), channel.Id()).c_str()));
        auto index = _outputs.find(typeid(WebSocketExporter));
        if (index != _outputs.end()) {
            auto instance = static_cast<WebSocketExporter*>(index->second.get());
            instance->Deactivate(channel);
        }
    }

    Core::ProxyType<Core::JSON::IElement> WarningReportingControl::Inbound(const string& identifier)
    {
        return WPEFramework::Core::proxy_cast<WPEFramework::Core::JSON::IElement>(jsonExportCommandFactory.Element());
    }

    Core::ProxyType<Core::JSON::IElement> WarningReportingControl::Inbound(const uint32_t ID, const Core::ProxyType<Core::JSON::IElement>& element)
    {
        auto index = _outputs.find(typeid(WebSocketExporter));
        if (index != _outputs.end()) {
            auto instance = static_cast<WebSocketExporter*>(index->second.get());
            return WPEFramework::Core::proxy_cast<WPEFramework::Core::JSON::IElement>(instance->HandleExportCommand(ID, element));
        }
    }

    string WarningReportingControl::Information() const
    {
        // No additional info to report.
        return (nullptr);
    }

    void WarningReportingControl::Dispatch(const WarningInformation& information)
    {
        ASSERT(information.event != nullptr);
        if (information.event != nullptr) {
            for (const auto& output : _outputs) {
                output.second->Output(information.filename.c_str(), information.lineNumber, information.identifier.c_str(), information.event.get());
            }
        }
    }
}
}
