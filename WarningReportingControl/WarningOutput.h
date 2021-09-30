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

    class WarningReportingTextOutput : public WarningReporting::IWarningReportingMedia {
    public:
        WarningReportingTextOutput(const WarningReportingTextOutput&) = delete;
        WarningReportingTextOutput& operator=(const WarningReportingTextOutput&) = delete;

        explicit WarningReportingTextOutput(const bool abbreviated)
            : _abbreviated(abbreviated)
        {
        }
        ~WarningReportingTextOutput() override = default;

    public:
        void Output(const char fileName[], const uint32_t lineNumber, const char identifer[], const WarningReporting::IWarningEvent* information) override
        {
            std::stringstream output;
            string message;
            information->ToString(message);
            output << (information->IsWarning() ? "[WARNING]" : "[REPORT]");

            if (_abbreviated == true) {
                string time(Core::Time::Now().ToTimeOnly(true));
                output << '[' << time.c_str() << "][" << identifer << "]: " << message << std::endl;
            } else {
                string time(Core::Time::Now().ToRFC1123(true));
                output << '[' << time.c_str() << "][" << identifer << "][" << Core::FileNameOnly(fileName) << ':' << lineNumber << "] " << information->Category()
                       << ": " << message << std::endl;
            }
            HandleTraceMessage(output.str());
        }

    private:
        virtual void HandleTraceMessage(const string& message) = 0;

    private:
        bool _abbreviated;
    };
    class WarningReportSyslogOutput : public WarningReportingTextOutput {
    public:
        WarningReportSyslogOutput(const WarningReportSyslogOutput&) = delete;
        WarningReportSyslogOutput& operator=(const WarningReportSyslogOutput&) = delete;

        explicit WarningReportSyslogOutput(const bool abbreviated)
            : WarningReportingTextOutput(abbreviated)
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

    class WarningReportConsoleOutput : public WarningReportingTextOutput {
    public:
        WarningReportConsoleOutput(const WarningReportConsoleOutput&) = delete;
        WarningReportConsoleOutput& operator=(const WarningReportConsoleOutput&) = delete;

        explicit WarningReportConsoleOutput(const bool abbreviated)
            : WarningReportingTextOutput(abbreviated)
        {
        }
        ~WarningReportConsoleOutput() override = default;

    private:
        void HandleTraceMessage(const string& message) override
        {
            printf(_T("%s"), message.c_str());
        }
    };

    class WarningReportFileOutput : public WarningReportingTextOutput {
    public:
        WarningReportFileOutput(const WarningReportFileOutput&) = delete;
        WarningReportFileOutput& operator=(const WarningReportFileOutput&) = delete;

        explicit WarningReportFileOutput(const bool abbreviated, const string& filepath)
            : WarningReportingTextOutput(abbreviated)
            , _file(filepath)
        {
            _file.Create();

            if (!_file.IsOpen()) {
                TRACE(Trace::Error, (_T("Could not open file <%s>. Outputting warnings to file unavailable."), filepath));
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

    class WarningReportingJSONOutput : public WarningReporting::IWarningReportingMedia {
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
                , Identifier()
                , Message()
            {
                Add(_T("time"), &Time);
                Add(_T("filename"), &Filename);
                Add(_T("linenumber"), &Linenumber);
                Add(_T("category"), &Category);
                Add(_T("identifier"), &Identifier);
                Add(_T("message"), &Message);
            }

            ~Data() override = default;

        public:
            Core::JSON::String Time;
            Core::JSON::String Filename;
            Core::JSON::DecUInt32 Linenumber;
            Core::JSON::String Category;
            Core::JSON::String Identifier;
            Core::JSON::String Message;
        };

    public:
        WarningReportingJSONOutput(const WarningReportingJSONOutput&) = delete;
        WarningReportingJSONOutput& operator=(const WarningReportingJSONOutput&) = delete;

        enum class ExtraOutputOptions {
            ABREVIATED = 0,
            FILENAME = 1,
            LINENUMBER = 3, // selecting LINENUMBER will automatically select FILENAME
            IDENTIFIER = 4,
            CATEGORY = 8,
            INCLUDINGDATE = 16,
            ALL = 31
        };

        explicit WarningReportingJSONOutput(WPEFramework::PluginHost::Channel& channel)
            : _exportChannel(channel)
            , _outputoptions(ExtraOutputOptions::ALL)
            , _paused(false)
        {
        }
        ~WarningReportingJSONOutput() override = default;

    public:
        bool IsPaused() const
        {
            return _paused;
        }

        ExtraOutputOptions OutputOptions() const
        {
            return _outputoptions;
        }

        void OutputOptions(const ExtraOutputOptions outputoptions)
        {
            _outputoptions = outputoptions;
        }

        template <typename E>
        static inline auto AsNumber(E t) -> typename std::underlying_type<E>::type
        {
            return static_cast<typename std::underlying_type<E>::type>(t);
        }

        void Output(const char fileName[], const uint32_t lineNumber, const char identifer[], const WarningReporting::IWarningEvent* information)
        {
            if (!IsPaused()) {
                ExtraOutputOptions options = _outputoptions;

                WPEFramework::Core::ProxyType<Data> data = GetDataContainer();
                data->Clear();
                if ((AsNumber(options) & AsNumber(ExtraOutputOptions::INCLUDINGDATE)) != 0) {
                    data->Time = WPEFramework::Core::Time::Now().ToRFC1123(true);
                } else {
                    data->Time = WPEFramework::Core::Time::Now().ToTimeOnly(true);
                }

                if ((AsNumber(options) & AsNumber(ExtraOutputOptions::FILENAME)) != 0) {
                    data->Filename = fileName;
                    if ((AsNumber(options) & AsNumber(ExtraOutputOptions::LINENUMBER)) != 0) {
                        data->Linenumber = lineNumber;
                    }
                }

                if ((AsNumber(options) & AsNumber(ExtraOutputOptions::IDENTIFIER)) != 0) {
                    data->Identifier = identifer;
                }

                if ((AsNumber(options) & AsNumber(ExtraOutputOptions::CATEGORY)) != 0) {
                    data->Category = information->Category();
                }

                string message;
                information->ToString(message);
                data->Message = message;

                HandleTraceMessage(data);
            }
        }
        virtual WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement> Process(const WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement>& element) = 0;

    private:
        virtual Core::ProxyType<Data> GetDataContainer() = 0;

        void HandleTraceMessage(const Core::ProxyType<Data>& jsondata)
        {
            _exportChannel.Submit(WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement>(jsondata));
        }

    protected:
        WPEFramework::PluginHost::Channel& _exportChannel;
        std::atomic<ExtraOutputOptions> _outputoptions;
        bool _paused;
    };
}
}
