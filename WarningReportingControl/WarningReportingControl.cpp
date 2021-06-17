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
            _outputs.emplace_back(new WarningReportConsoleOutput(_config.Abbreviated.Value()));
        }
        if (((service->Background() == true) && (_config.Console.IsSet() == false) && (_config.SysLog.IsSet() == false))
            || ((_config.SysLog.IsSet() == true) && (_config.SysLog.Value() == true))) {
            _outputs.emplace_back(new WarningReportSyslogOutput(_config.Abbreviated.Value()));
        }
        if(_config.FilePath.IsSet()){
        _outputs.emplace_back(new WarningReportFileOutput(_config.Abbreviated.Value(), _config.FilePath.Value()));
        }

        _service->Register(&_observer);

        // Start observing..
        _observer.Start();

        // On succes return a name as a Callsign to be used in the URL, after the "service"prefix
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

    string WarningReportingControl::Information() const
    {
        // No additional info to report.
        return (nullptr);
    }

    void WarningReportingControl::Dispatch(const WarningInformation& information)
    {
        ASSERT(information.event != nullptr);
        if (information.event != nullptr) {
            for(const auto& output: _outputs){
                output->Output(information.filename.c_str(), information.lineNumber, information.identifier.c_str(), information.event.get());
            }
        }
    }
}
}
