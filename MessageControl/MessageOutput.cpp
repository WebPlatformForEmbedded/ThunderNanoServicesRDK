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

namespace Thunder {

namespace Publishers {

    string Text::Convert(const Core::Messaging::MessageInfo& metadata, const string& text) /* override */
    {
        ASSERT(metadata.Type() != Core::Messaging::Metadata::type::INVALID);

        string output = metadata.ToString(_abbreviated);

        output.reserve(output.size() + text.size() + 1);

        output.append(text);
        output.push_back('\n');

        return (output);
    }

    void ConsoleOutput::Message(const Core::Messaging::MessageInfo& metadata, const string& text) /* override */
    {
        Messaging::ConsoleStandardOut::Instance().Format(_convertor.Convert(metadata, text).c_str());
    }

    void SyslogOutput::Message(const Core::Messaging::MessageInfo& metadata, const string& text) /* override */
    {
#ifndef __WINDOWS__
        syslog(LOG_NOTICE, _T("%s"), _convertor.Convert(metadata, text).c_str());
#else
        Messaging::ConsoleStandardOut::Instance().Format(_convertor.Convert(metadata, text).c_str());
#endif
    }

    void FileOutput::Message(const Core::Messaging::MessageInfo& metadata, const string& text) /* override */
    {
        if (_file.IsOpen()) {
            const string line = _convertor.Convert(metadata, text);
            _file.Write(reinterpret_cast<const uint8_t*>(line.c_str()), static_cast<uint32_t>(line.length()));
        }
    }

    void JSON::Convert(const Core::Messaging::MessageInfo& metadata, const string& text, Data& data)
    {
        ExtraOutputOptions options = _outputOptions;

        if ((AsNumber(options) & AsNumber(ExtraOutputOptions::PAUSED)) == 0) {

            if ((AsNumber(options) & AsNumber(ExtraOutputOptions::CATEGORY)) != 0) {
                data.Category = metadata.Category();
            }

            if ((AsNumber(options) & AsNumber(ExtraOutputOptions::MODULE)) != 0) {
                data.Module = metadata.Module();
            }

            const Core::Time now(metadata.TimeStamp());
            if ((AsNumber(options) & AsNumber(ExtraOutputOptions::INCLUDINGDATE)) != 0) {
                data.Time = now.ToRFC1123(true);
            }
            else {
                data.Time = now.ToTimeOnly(true);
            }

            if (metadata.Type() == Core::Messaging::Metadata::type::TRACING) {
                ASSERT(dynamic_cast<const Core::Messaging::IStore::Tracing*>(&metadata) != nullptr);
                const Core::Messaging::IStore::Tracing& trace = static_cast<const Core::Messaging::IStore::Tracing&>(metadata);

                if ((AsNumber(options) & AsNumber(ExtraOutputOptions::FILENAME)) != 0) {
                    data.FileName = trace.FileName();
                }

                if ((AsNumber(options) & AsNumber(ExtraOutputOptions::LINENUMBER)) != 0) {
                    data.LineNumber = trace.LineNumber();
                }

                if ((AsNumber(options) & AsNumber(ExtraOutputOptions::CLASSNAME)) != 0) {
                    data.ClassName = trace.ClassName();
                }
            }
            else if (metadata.Type() == Core::Messaging::Metadata::type::REPORTING) {
                ASSERT(dynamic_cast<const Core::Messaging::IStore::WarningReporting*>(&metadata) != nullptr);
                const Core::Messaging::IStore::WarningReporting& report = static_cast<const Core::Messaging::IStore::WarningReporting&>(metadata);

                if ((AsNumber(options) & AsNumber(ExtraOutputOptions::CALLSIGN)) != 0) {
                    data.Callsign = report.Callsign();
                }
            }
            else if (metadata.Type() == Core::Messaging::Metadata::type::ASSERT) {
                ASSERT(dynamic_cast<const Core::Messaging::IStore::Assert*>(&metadata) != nullptr);
                const Core::Messaging::IStore::Assert& assert = static_cast<const Core::Messaging::IStore::Assert&>(metadata);

                if ((AsNumber(options) & AsNumber(ExtraOutputOptions::PROCESSID)) != 0) {
                    data.ProcessId = assert.ProcessId();
                }

                if ((AsNumber(options) & AsNumber(ExtraOutputOptions::PROCESSNAME)) != 0) {
                    data.ProcessName = assert.ProcessName();
                }
                if ((AsNumber(options) & AsNumber(ExtraOutputOptions::FILENAME)) != 0) {
                    data.FileName = assert.FileName();
                }

                if ((AsNumber(options) & AsNumber(ExtraOutputOptions::LINENUMBER)) != 0) {
                    data.LineNumber = assert.LineNumber();
                }
            }
            else {
                ASSERT(metadata.Type() != Core::Messaging::Metadata::type::INVALID);
            }

            data.Message = text;
        }
    }

    //UDPOutput
    UDPOutput::Channel::Channel(const Core::NodeId& nodeId)
        : Core::SocketDatagram(false, nodeId.Origin(), nodeId, Messaging::MessageUnit::Instance().DataSize(), 0)
        , _queue()
    {
    }
    UDPOutput::Channel::~Channel()
    {
        Close(Core::infinite);
    }

    uint16_t UDPOutput::Channel::SendData(uint8_t* dataFrame, const uint16_t maxSendSize)
    {
        uint16_t actualByteCount = 0;

        _adminLock.Lock();

        if (_queue.empty() == true) {
            _adminLock.Unlock();
        }
        else {
            string msg = std::move(_queue.front());
            _queue.pop();
            _adminLock.Unlock();

            actualByteCount = std::min<uint16_t>(msg.size(), maxSendSize);
            memcpy(dataFrame, msg.c_str(), actualByteCount);
        }

        return (actualByteCount);
    }

    //unused
    uint16_t UDPOutput::Channel::ReceiveData(uint8_t*, const uint16_t) {
        return 0;
    }

    void UDPOutput::Channel::StateChange()
    {
    }

    void UDPOutput::Channel::Output(string&& text)
    {
        _adminLock.Lock();

        _queue.emplace(std::move(text));

        _adminLock.Unlock();

        Trigger();
    }

    UDPOutput::UDPOutput(const Core::Messaging::MessageInfo::abbreviate abbreviate, const Core::NodeId& nodeId, PluginHost::IShell* service, const string& interface)
        : _convertor(abbreviate)
        , _output(nodeId)
        , _notification(*this)
        , _subSystem(service->SubSystems())
        , _interface(interface)
    {
        ASSERT(_subSystem != nullptr);

        if (_subSystem != nullptr) {
            _subSystem->AddRef();
            _subSystem->Register(&_notification);
        }
    }

    void UDPOutput::UpdateChannel()
    {
        if (_subSystem->IsActive(PluginHost::ISubSystem::NETWORK)) {
            if (_output.IsOpen() == false) {
                if (_interface.empty() == false) {
                    _output.Open(0, _interface);
                }
                else {
                    _output.Open(0);
                }
            }
            ASSERT(_output.IsOpen() == true);
        }
        else {
            _output.Close(Core::infinite);
        }
    }

    void UDPOutput::Message(const Core::Messaging::MessageInfo& metadata, const string& text) /* override */
    {
        if (_output.IsOpen() == true) {
            _output.Output(_convertor.Convert(metadata, text));
        }
    }

} // namespace Publishers
}
