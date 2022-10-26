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

namespace WPEFramework {
namespace Publishers {

    string Text::Convert(const Core::Messaging::IStore::Information& info, const Core::Messaging::IEvent* message)
    {
        string output;

        const Core::Time now(info.TimeStamp());

        if (_abbreviated == true) {
            const string time(now.ToTimeOnly(true));
            output = Core::Format("[%s]:[%s]:[%s]: %s\n",
                    time.c_str(),
                    info.Module().c_str(),
                    info.Category().c_str(),
                    message->Data().c_str());
        } else {
            const string time(now.ToRFC1123(true));
            output = Core::Format("[%s]:[%s:%u]:[%s]:[%s]: %s\n",
                    time.c_str(),
                    Core::FileNameOnly(info.FileName().c_str()),
                    info.LineNumber(),
                    info.ClassName().c_str(),
                    info.Category().c_str(),
                    message->Data().c_str());
        }

        return (output);
    }

    void ConsoleOutput::Output(const Core::Messaging::IStore::Information& info, const Core::Messaging::IEvent* message) /* override */
    {
        std::cout << _convertor.Convert(info, message);
    }

    void SyslogOutput::Output(const Core::Messaging::IStore::Information& info, const Core::Messaging::IEvent* message) /* override */
    {
#ifndef __WINDOWS__
        syslog(LOG_NOTICE, _T("%s"), _convertor.Convert(info, message).c_str());
#else
        printf(_T("%s"), _convertor.Convert(info, message).c_str());
#endif
    }

    void FileOutput::Output(const Core::Messaging::IStore::Information& info, const Core::Messaging::IEvent* message)
    {
        if (_file.IsOpen()) {
            string line = _convertor.Convert(info, message);
            _file.Write(reinterpret_cast<const uint8_t*>(line.c_str()), static_cast<uint32_t>(line.length()));
        }
    }

    void JSON::Convert(const Core::Messaging::IStore::Information& info, const Core::Messaging::IEvent* message, Data& data)
    {
        ExtraOutputOptions options = _outputOptions;

        if ((AsNumber(options) & AsNumber(ExtraOutputOptions::PAUSED)) == 0) {

            if ((AsNumber(options) & AsNumber(ExtraOutputOptions::INCLUDINGDATE)) != 0) {
                data.Time = Core::Time::Now().ToRFC1123(true);
            } else {
                data.Time = Core::Time::Now().ToTimeOnly(true);
            }

            if ((AsNumber(options) & AsNumber(ExtraOutputOptions::FILENAME)) != 0) {
                data.FileName = info.FileName();
            }

            if ((AsNumber(options) & AsNumber(ExtraOutputOptions::LINENUMBER)) != 0) {
                data.LineNumber = info.LineNumber();
            }

            if( (AsNumber(options) & AsNumber(ExtraOutputOptions::CLASSNAME) ) != 0 ) {
                data.ClassName = info.ClassName();
            }

            if ((AsNumber(options) & AsNumber(ExtraOutputOptions::MODULE)) != 0) {
                data.Module = info.Module();
            }

            if ((AsNumber(options) & AsNumber(ExtraOutputOptions::CATEGORY)) != 0) {
                data.Category = info.Category();
            }

            data.Message = message->Data();
        }
    }

    //UDPOutput
    UDPOutput::Channel::Channel(const Core::NodeId& nodeId)
        : Core::SocketDatagram(false, nodeId.Origin(), nodeId, Messaging::MessageUnit::DataSize, 0)
        , _loaded(0)
    {
    }
    UDPOutput::Channel::~Channel()
    {
        Close(Core::infinite);
    }

    uint16_t UDPOutput::Channel::SendData(uint8_t* dataFrame, const uint16_t maxSendSize)
    {
        _adminLock.Lock();

        uint16_t actualByteCount = (_loaded > maxSendSize ? maxSendSize : _loaded);
        memcpy(dataFrame, _sendBuffer, actualByteCount);
        _loaded = 0;

        _adminLock.Unlock();
        return (actualByteCount);
    }

    //unused
    uint16_t UDPOutput::Channel::ReceiveData(uint8_t*, const uint16_t)
    {
        return 0;
    }
    void UDPOutput::Channel::StateChange()
    {
    }

    void UDPOutput::Channel::Output(const Core::Messaging::IStore::Information& info, const Core::Messaging::IEvent* message)
    {
        _adminLock.Lock();

        uint16_t length = 0;
        length += info.Serialize(_sendBuffer + length, sizeof(_sendBuffer) - length);
        length += message->Serialize(_sendBuffer + length, sizeof(_sendBuffer) - length);
        _loaded = length;

        _adminLock.Unlock();

        Trigger();
    }

    UDPOutput::UDPOutput(const Core::NodeId& nodeId)
        : _output(nodeId)
    {
        _output.Open(0);
    }

    void UDPOutput::Output(const Core::Messaging::IStore::Information& info, const Core::Messaging::IEvent* message)
    {
        _output.Output(info, message);
    }
}
}
