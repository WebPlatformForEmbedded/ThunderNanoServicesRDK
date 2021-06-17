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

#pragma once

#include "Module.h"

#ifndef __WINDOWS__
#include <syslog.h>
#endif

#include "WarningReportingControl.h"
#include <sstream>
#include <type_traits>

namespace WPEFramework {

namespace Plugin {

    class WarningReportOutput : public WarningReporting::IWarningReportingMedia {
    public:
        WarningReportOutput(const WarningReportOutput&) = delete;
        WarningReportOutput& operator=(const WarningReportOutput&) = delete;

        explicit WarningReportOutput(const bool abbreviated)
            : _abbreviated(abbreviated)
        {
        }
        ~WarningReportOutput() override = default;

    public:
        void Output(const char fileName[], const uint32_t lineNumber, const char identifer[], const WarningReporting::IWarningEvent* information) override
        {
            std::stringstream output;
            string message;
            information->ToString(message);

            if (_abbreviated == true) {
                string time(Core::Time::Now().ToTimeOnly(true));
                output << '[' << time.c_str() << "]: " << message << std::endl;
            } else {
                string time(Core::Time::Now().ToRFC1123(true));
                output << '[' << time.c_str() << "]:[" << identifer << "]:[" << Core::FileNameOnly(fileName) << ':' << lineNumber << "] " << information->Category()
                       << ": " << message << std::endl;
            }
            HandleTraceMessage(output.str());
        }

    private:
        virtual void HandleTraceMessage(const string& message) = 0;

    private:
        bool _abbreviated;
    };
    class WarningReportSyslogOutput : public WarningReportOutput {
    public:
        WarningReportSyslogOutput(const WarningReportSyslogOutput&) = delete;
        WarningReportSyslogOutput& operator=(const WarningReportSyslogOutput&) = delete;

        explicit WarningReportSyslogOutput(const bool abbreviated)
            : WarningReportOutput(abbreviated)
        {
        }

        ~WarningReportSyslogOutput() override = default;

    private:
        void HandleTraceMessage(const string& message) override
        {
#ifndef __WINDOWS__
            syslog(LOG_NOTICE, _T("%s"), message.c_str());
#else
            printf(_T("%s"), message.c_str());
#endif
        }
    };

    class WarningReportConsoleOutput : public WarningReportOutput {
    public:
        WarningReportConsoleOutput(const WarningReportConsoleOutput&) = delete;
        WarningReportConsoleOutput& operator=(const WarningReportConsoleOutput&) = delete;

        explicit WarningReportConsoleOutput(const bool abbreviated)
            : WarningReportOutput(abbreviated)
        {
        }
        ~WarningReportConsoleOutput() override = default;

    private:
        void HandleTraceMessage(const string& message) override
        {
            printf(_T("%s"), message.c_str());
        }
    };

    class WarningReportFileOutput : public WarningReportOutput {
    public:
        WarningReportFileOutput(const WarningReportFileOutput&) = delete;
        WarningReportFileOutput& operator=(const WarningReportFileOutput&) = delete;

        explicit WarningReportFileOutput(const bool abbreviated, const string& filepath)
            : WarningReportOutput(abbreviated)
            , _file(filepath)
        {
            _file.Create();

            if (!_file.IsOpen()) {
                TRACE(Trace::Error, (_T("Could not open file <%s>. Outputing warnings to file unavailable."), filepath));
            }
        }
        ~WarningReportFileOutput() override
        {
            if (_file.IsOpen()) {
                _file.Close();
            }
        }

    private:
        void HandleTraceMessage(const string& message) override
        {
            if (_file.IsOpen()) {
                _file.Write(reinterpret_cast<const uint8_t*>(message.c_str()), message.length());
            }
        }

    private:
        Core::File _file;
    };
}
}
