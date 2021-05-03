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
 
#include "TraceControl.h"
#include "TraceOutput.h"

#include <algorithm>

namespace {

    WPEFramework::Core::ProxyPoolType<WPEFramework::Web::JSONBodyType<WPEFramework::Plugin::TraceControl::Data>> jsonBodyDataFactory(4);
    WPEFramework::Core::ProxyPoolType<WPEFramework::Plugin::TraceJSONOutput::Data> jsonExportDataFactory(2);

    class WebSocketExporter : public WPEFramework::Plugin::TraceJSONOutput {

    public:
        class ExportCommand : public WPEFramework::Core::JSON::Container {

        public:
            ExportCommand(const ExportCommand&) = delete;
            ExportCommand& operator=(const ExportCommand&) = delete;

            ExportCommand()
                : WPEFramework::Core::JSON::Container()
                , Filename()
                , Classname()
                , Category()
                , IncludingDate()
                , Paused()
            {
                Add(_T("filename"), &Filename);
                Add(_T("classname"), &Classname);
                Add(_T("category"), &Category);
                Add(_T("includingdate"), &IncludingDate);
                Add(_T("paused"), &Paused);
            }

            ~ExportCommand() override = default;

        public:
            WPEFramework::Core::JSON::Boolean Filename;
            WPEFramework::Core::JSON::Boolean Classname;
            WPEFramework::Core::JSON::Boolean Category;
            WPEFramework::Core::JSON::Boolean IncludingDate;
            WPEFramework::Core::JSON::Boolean Paused;
        };

    public:
        class TraceChannelOutput {

        public:
            TraceChannelOutput(const TraceChannelOutput&) = delete;
            TraceChannelOutput& operator=(const TraceChannelOutput&) = delete;

            explicit TraceChannelOutput(WPEFramework::PluginHost::Channel& channel)
                : _exportChannel(&channel)
                , _outputoptions(ExtraOutputOptions::ALL)
                , _paused(false) {

            }

            ~TraceChannelOutput() = default;

            void HandleTraceMessage(const WPEFramework::Core::ProxyType<Data>& jsondata) {
                _exportChannel->Submit(WPEFramework::Core::proxy_cast<WPEFramework::Core::JSON::IElement>(jsondata));
            }

            WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement> Process(const WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement>& element);

            void Output(const char fileName[], const uint32_t lineNumber, const char className[], const WPEFramework::Trace::ITrace* information) {
                ExtraOutputOptions options = _outputoptions;

                WPEFramework::Core::ProxyType<Data> data = GetDataContainer();
                data->Clear();
                if( ( AsNumber(options) & AsNumber(ExtraOutputOptions::INCLUDINGDATE) ) != 0 ) {
                    data->Time = WPEFramework::Core::Time::Now().ToRFC1123(true);
                } else {
                    data->Time = WPEFramework::Core::Time::Now().ToTimeOnly(true);
                }

                if( ( AsNumber(options) & AsNumber(ExtraOutputOptions::FILENAME) ) != 0 ) {
                    data->Filename = fileName;
                    if( ( AsNumber(options) & AsNumber(ExtraOutputOptions::LINENUMBER) ) != 0 ) {
                        data->Linenumber = lineNumber;
                    }
                }

                if( ( AsNumber(options) & AsNumber(ExtraOutputOptions::CLASSNAME) ) != 0 ) {
                    data->Classname = className;
                }

                if( ( AsNumber(options) & AsNumber(ExtraOutputOptions::CATEGORY) ) != 0 ) {
                    data->Category = information->Category();
                }

                data->Message = information->Data();

                HandleTraceMessage(data);
            }

            bool IsPaused() const {
                return _paused;
            }

            WPEFramework::Core::ProxyType<Data> GetDataContainer() {
                return jsonExportDataFactory.Element();
            }

            ExtraOutputOptions OutputOptions() const {
                return _outputoptions;
            }

            void OutputOptions(const ExtraOutputOptions outputoptions) {
                _outputoptions = outputoptions;
            }

        private :
            WPEFramework::PluginHost::Channel* _exportChannel;
            std::atomic<ExtraOutputOptions>    _outputoptions;
            bool                               _paused;
        };


    public :
        WebSocketExporter(const WebSocketExporter& copy) = delete;
        WebSocketExporter& operator=(const WebSocketExporter&) = delete;

        static WebSocketExporter &Instance()
        {
            static WebSocketExporter _instance;
            return (_instance);
        }

        WebSocketExporter()
            : _lock() {
        }

        ~WebSocketExporter() override = default;

        bool Activate(WPEFramework::PluginHost::Channel& channel) {

            _lock.Lock();

            ASSERT(0 == _traceChannelOutputs.count(channel.Id()));
            _traceChannelOutputs.emplace(std::make_pair(channel.Id(), new TraceChannelOutput(channel)));

            _lock.Unlock();

            return true;
        }

        bool Deactivate(WPEFramework::PluginHost::Channel& channel) {

            bool deactivated = false;

            if ( 0 != _traceChannelOutputs.count(channel.Id())) {

                _lock.Lock();

                _traceChannelOutputs.erase(channel.Id());

                deactivated = true;

                _lock.Unlock();
            }

            return deactivated;

        }

        WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement> HandleExportCommand(const uint32_t ID, const WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement>& element);

        uint16_t ActiveChannelCount() const {
            return static_cast<uint16_t>(_traceChannelOutputs.size());
        }

        bool IsActive(const uint32_t ID) const {
            return (_traceChannelOutputs.count(ID) > 0) ? true : false;
        }

        void Output(const char fileName[], const uint32_t lineNumber, const char className[], const WPEFramework::Trace::ITrace* information) override {

            _lock.Lock();

            for (auto& item : _traceChannelOutputs) {
                if (!item.second->IsPaused())
                    item.second->Output(fileName, lineNumber, className, information);
            }

            _lock.Unlock();
        }

    private:
        void HandleTraceMessage(const WPEFramework::Core::ProxyType<Data>& jsondata) override {
            _lock.Lock();

            for (auto& item : _traceChannelOutputs) {
                item.second->HandleTraceMessage(jsondata);
            }

            _lock.Unlock();

        }

        WPEFramework::Core::ProxyType<Data> GetDataContainer() override {
            return jsonExportDataFactory.Element();
        }

    private:
        using TraceChannelOutputPtr = std::unique_ptr<WebSocketExporter::TraceChannelOutput>;

        std::unordered_map<uint32_t, TraceChannelOutputPtr>   _traceChannelOutputs;
        mutable WPEFramework::Core::CriticalSection           _lock;
    };

    WPEFramework::Core::ProxyPoolType<WebSocketExporter::ExportCommand> jsonExportCommandFactory(2);


    WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement> WebSocketExporter::HandleExportCommand(const uint32_t ID,
                                       const WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement>& element) {

        WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement> response;

        _lock.Lock();

        if (IsActive(ID))
            response = (_traceChannelOutputs[ID]->Process(element));

        _lock.Unlock();

        return response;
    }


    WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement> WebSocketExporter::TraceChannelOutput::Process(const WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement>& element) {
        WPEFramework::Core::ProxyType<WebSocketExporter::ExportCommand>
                            inbound(WPEFramework::Core::proxy_cast<WebSocketExporter::ExportCommand>(element));

        ASSERT(inbound.IsValid() == true);

        ExtraOutputOptions options = OutputOptions();

        if( inbound->Filename.IsSet() == true ) {
            if( inbound->Filename == true ) {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) | AsNumber(ExtraOutputOptions::LINENUMBER));
            } else {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) & ~AsNumber(ExtraOutputOptions::LINENUMBER));
            }
        }

        if( inbound->Classname.IsSet() == true ) {
            if( inbound->Classname == true ) {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) | AsNumber(ExtraOutputOptions::CLASSNAME));
            } else {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) & ~AsNumber(ExtraOutputOptions::CLASSNAME));
            }
        }

        if( inbound->Category.IsSet() == true ) {
            if( inbound->Category == true ) {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) | AsNumber(ExtraOutputOptions::CATEGORY));
            } else {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) & ~AsNumber(ExtraOutputOptions::CATEGORY));
            }
        }

        if( inbound->IncludingDate.IsSet() == true ) {
            if( inbound->IncludingDate == true ) {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) | AsNumber(ExtraOutputOptions::INCLUDINGDATE));
            } else {
                options = static_cast<ExtraOutputOptions>(AsNumber(options) & ~AsNumber(ExtraOutputOptions::INCLUDINGDATE));
            }
        }

        OutputOptions(options);

        if( inbound->Paused.IsSet() == true ) {
            _paused = inbound->Paused;
        }

        WPEFramework::Core::ProxyType<ExportCommand> response(jsonExportCommandFactory.Element());

        response->Filename = ( ( AsNumber(options) & AsNumber(ExtraOutputOptions::FILENAME) ) != 0 );
        response->Classname = ( ( AsNumber(options) & AsNumber(ExtraOutputOptions::CLASSNAME) ) != 0 );
        response->Category = ( ( AsNumber(options) & AsNumber(ExtraOutputOptions::CATEGORY) ) != 0 );
        response->IncludingDate = ( ( AsNumber(options) & AsNumber(ExtraOutputOptions::INCLUDINGDATE) ) != 0 );
        response->Paused = IsPaused();

        return WPEFramework::Core::proxy_cast<WPEFramework::Core::JSON::IElement>(response);
    }


}

namespace WPEFramework {

ENUM_CONVERSION_BEGIN(Plugin::TraceControl::state)

    { Plugin::TraceControl::state::ENABLED, _TXT("enabled") },
    { Plugin::TraceControl::state::DISABLED, _TXT("disabled") },
    { Plugin::TraceControl::state::TRISTATED, _TXT("tristated") },

    ENUM_CONVERSION_END(Plugin::TraceControl::state);

namespace Plugin {

    SERVICE_REGISTRATION(TraceControl, 1, 0);

    // Declare the local trace iterator exposing the remote interface.
    /* static */ TraceControl::Observer::Source::LocalIterator TraceControl::Observer::Source::_localIterator;

    /* static */ string TraceControl::Observer::Source::SourceName(const string& prefix, RPC::IRemoteConnection* connection)
    {
        string pathName;
        pathName = Core::Directory::Normalize(prefix) + Trace::CyclicBufferName;

        if (connection != nullptr) {
            pathName += '.' + Core::NumberType<uint32_t>(connection->Id()).Text();
        }

        return (pathName);
    }

    const string TraceControl::Initialize(PluginHost::IShell* service)
    {
        ASSERT(_service == nullptr);
        ASSERT(_outputs.size() == 0);

        _service = service;
        _config.FromString(_service->ConfigLine());
        _tracePath = service->VolatilePath();

        _maxExportConnections = _config.MaxExportConnections.Value();

        size_t pos = service->Callsign().length(); 
        if ( (pos = _tracePath.find_last_of('/', (_tracePath.length() >= pos ? _tracePath.length() - pos : string::npos))) != string::npos ) 
        {
            _tracePath = _tracePath.substr(0, pos);
        }

        _skipURL = static_cast<uint8_t>(_service->WebPrefix().length());

        auto customDeleter = [](Trace::ITraceMedia *pObj) {delete pObj;};

        if (((service->Background() == false) && (_config.Console.IsSet() == false) && (_config.SysLog.IsSet() == false)) || ((_config.Console.IsSet() == true) && (_config.Console.Value() == true))) {
            _outputs.emplace_back(std::unique_ptr<Trace::ITraceMedia, Custom_deleter>(new TraceConsoleOutput(false), customDeleter));
        }
        if (((service->Background() == true) && (_config.Console.IsSet() == false) && (_config.SysLog.IsSet() == false)) || ((_config.SysLog.IsSet() == true) && (_config.SysLog.Value() == true))) {
            _outputs.emplace_back(std::unique_ptr<Trace::ITraceMedia, Custom_deleter>(new TraceSyslogOutput(_config.Abbreviated.Value()), customDeleter));
        }
        if (_config.Remote.IsSet() == true) {
            Core::NodeId logNode(_config.Remote.Binding.Value().c_str(), _config.Remote.Port.Value());

            _outputs.emplace_back(std::unique_ptr<Trace::ITraceMedia, Custom_deleter>(new Trace::TraceMedia(logNode), customDeleter));
        }

        // put the WebSocketExporter last so we can find it there if needed
        if( _maxExportConnections != 0 ) {
            std::unique_ptr<Trace::ITraceMedia, Custom_deleter> obj{&(WebSocketExporter::Instance()), [](Trace::ITraceMedia*){
                //NOP
            }};

            _outputs.push_back(std::move(obj));
        }

        _service->Register(&_observer);

        // Start observing..
        _observer.Start();

        // On succes return a name as a Callsign to be used in the URL, after the "service"prefix
        return (_T(""));
    }

    void TraceControl::Deinitialize(PluginHost::IShell* service VARIABLE_IS_NOT_USED)
    {
        ASSERT(service == _service);

        _service->Unregister(&_observer);

        // Stop observing..
        _observer.Stop();

        _outputs.clear();

    }

    string TraceControl::Information() const
    {
        // No additional info to report.
        return (nullptr);
    }

    bool TraceControl::Attach(PluginHost::Channel& channel) {

        bool accepted = false;

        auto& webSocketExporterInstance = WebSocketExporter::Instance();

        if( (_maxExportConnections !=0) && (_maxExportConnections - webSocketExporterInstance.ActiveChannelCount() > 0) ) {
                TRACE(Trace::Information, (Core::Format(_T("Opened export connection channel ID [%d]"), channel.Id()).c_str()));
                accepted = webSocketExporterInstance.Activate(channel);
        }

        return accepted;
    }

    void TraceControl::Detach(PluginHost::Channel& channel) {

        auto& webSocketExporterInstance = WebSocketExporter::Instance();

        if( (_maxExportConnections !=0) && (webSocketExporterInstance.ActiveChannelCount() > 0)) {
            if(webSocketExporterInstance.Deactivate(channel) == true) {
                TRACE(Trace::Information, (Core::Format(_T("Closed export connection channel ID [%d]"), channel.Id()).c_str()));
            }
        }
    }

    WPEFramework::Core::ProxyPoolType<WebSocketExporter::ExportCommand> jsonExportCommandFactory(2);

    Core::ProxyType<Core::JSON::IElement> TraceControl::Inbound(const string& identifier) {
        return WPEFramework::Core::proxy_cast<WPEFramework::Core::JSON::IElement>(jsonExportCommandFactory.Element());
    }

    Core::ProxyType<Core::JSON::IElement> TraceControl::Inbound(const uint32_t ID, const Core::ProxyType<Core::JSON::IElement>& element) {

        Core::ProxyType<Core::JSON::IElement> response;

        auto& webSocketExporterInstance = WebSocketExporter::Instance();

        if ( (_maxExportConnections !=0) && (webSocketExporterInstance.IsActive(ID)) ) {
            response = WPEFramework::Core::proxy_cast<WPEFramework::Core::JSON::IElement>(webSocketExporterInstance.HandleExportCommand(ID, element));
        }

        return response;
    }
        
    void TraceControl::Inbound(Web::Request& /* request */)
    {
        // There are no requests coming in with a body that require a JSON body. So continue without action here.
    }

    // <GET> ../
    // <PUT> ../[on,off]
    // <PUT> ../<ModuleName>/[on,off]
    // <PUT> ../<ModuleName>/<CategoryName>/[on,off]
    Core::ProxyType<Web::Response> TraceControl::Process(const Web::Request& request)
    {
        ASSERT(_skipURL <= request.Path.length());

        Core::ProxyType<Web::Response> result(PluginHost::IFactories::Instance().Response());
        Core::TextSegmentIterator index(Core::TextFragment(request.Path, _skipURL, static_cast<uint32_t>(request.Path.length()) - _skipURL), false, '/');

        // If there is an entry, the first one will alwys be a '/', skip this one..
        index.Next();

        // By default, we assume everything works..
        result->ErrorCode = Web::STATUS_OK;
        result->Message = "OK";

        if (request.Verb == Web::Request::HTTP_GET) {
            Core::ProxyType<Web::JSONBodyType<TraceControl::Data>> response(jsonBodyDataFactory.Element());
            // Nothing more required, just return the current status...
            response->Console = _config.Console;
            response->Remote = _config.Remote;

            Observer::ModuleIterator index(_observer.Modules());

            while (index.Next() == true) {
                string moduleName(Core::ToString(index.Module()));

                Observer::ModuleIterator::CategoryIterator categories(index.Categories());

                while (categories.Next()) {
                    string categoryName(Core::ToString(categories.Category()));

                    response->Settings.Add(Data::Trace(moduleName, categoryName, categories.State()));
                }
            }

            result->Body(Core::proxy_cast<Web::IBody>(response));
            result->ContentType = Web::MIME_JSON;
        } else if ((request.Verb == Web::Request::HTTP_PUT) && (index.Next() == true)) {
            if ((index.Current() == _T("on")) || (index.Current() == _T("off"))) {
                // Done, no options, enable/disable all
                _observer.Set(
                    (index.Current() == _T("on")),
                    string(EMPTY_STRING),
                    string(EMPTY_STRING));
            } else {
                std::string moduleName(Core::ToString(index.Current().Text()));

                if (index.Next() == true) {
                    if ((index.Current() == _T("on")) || (index.Current() == _T("off"))) {
                        // Done, only a modulename is set, enable/disable all
                        _observer.Set(
                            (index.Current() == _T("on")),
                            (moduleName.length() != 0 ? moduleName : std::string(EMPTY_STRING)),
                            std::string(EMPTY_STRING));
                    } else {
                        std::string categoryName(Core::ToString(index.Current().Text()));

                        if (index.Next() == true) {
                            if ((index.Current() == _T("on")) || (index.Current() == _T("off"))) {
                                // Done, only a modulename is set, enable/disable all
                                _observer.Set(
                                    (index.Current() == _T("on")),
                                    (moduleName.length() != 0 ? moduleName : std::string(EMPTY_STRING)),
                                    (categoryName.length() != 0 ? categoryName : std::string(EMPTY_STRING)));
                            } else {
                                result->ErrorCode = Web::STATUS_BAD_REQUEST;
                                result->Message = _T(" could not handle your request, last parameter should be [on,off].");
                            }
                        } else {
                            result->ErrorCode = Web::STATUS_BAD_REQUEST;
                            result->Message = _T(" could not handle your request, last parameter should be the category.");
                        }
                    }
                } else {
                    result->ErrorCode = Web::STATUS_BAD_REQUEST;
                    result->Message = _T(" could not handle your request.");
                }
            }
        } else {
            result->ErrorCode = Web::STATUS_BAD_REQUEST;
            result->Message = _T(" could not handle your request.");
        }

        // Make sure all interface aquired for this action are released again otherwise
        // a process can not shut down if the plugin is deactivated due to an trace
        // interface still referenced !!!!
        _observer.Relinquish();

        return (result);
    }

    void TraceControl::Dispatch(Observer::Source& information)
    {
        InformationWrapper wrapper(information);

        for (auto& traceMedia : _outputs) {
            (traceMedia.get())->Output(information.FileName(), information.LineNumber(), information.ClassName(), &wrapper);
        }
    }
}
}
