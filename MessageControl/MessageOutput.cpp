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
namespace Messaging {

    TextOutput::TextOutput(const bool abbreviated)
        : _abbreviated(abbreviated)
    {
    }

    void TextOutput::Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message)
    {
        _output.str("");
        _output.clear();

        message->ToString(_deserializedMessage);
        Core::Time now(info.TimeStamp());

        if (_abbreviated == true) {
            string time(now.ToTimeOnly(true));
            _output << '[' << time.c_str() << ']' << '[' << info.MessageMetaData().Category() << "]: " << _deserializedMessage << std::endl;
        } else {
            string time(now.ToRFC1123(true));
            _output << '[' << time.c_str() << "]:[" << Core::FileNameOnly(info.FileName().c_str()) << ':' << info.LineNumber() << "] "
                    << info.MessageMetaData().Category() << ": " << _deserializedMessage << std::endl;
        }

        HandleTextMessage(_output.str());
    }

    void ConsoleOutput::HandleTextMessage(const string& message)
    {
        std::cout << message;
    }

    void SyslogOutput::HandleTextMessage(const string& message)
    {
#ifndef __WINDOWS__
        syslog(LOG_NOTICE, _T("%s"), message.c_str());
#else
        printf(_T("%s"), message.c_str());
#endif
    }

    FileOutput::FileOutput(bool abbreviate, const string& filepath)
        : TextOutput(abbreviate)
        , _file(filepath)
    {
        _file.Create();

        if (!_file.IsOpen()) {
            TRACE(Trace::Error, (_T("Could not open file <%s>. Outputting warnings to file unavailable."), filepath));
        }
    }
    FileOutput::~FileOutput()
    {
        if (_file.IsOpen()) {
            _file.Close();
        }
    }

    void FileOutput::HandleTextMessage(const string& message)
    {
        if (_file.IsOpen()) {
            _file.Write(reinterpret_cast<const uint8_t*>(message.c_str()), message.length());
        }
    }

    JSONOutput::Data::Data()
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

    JSONOutput::JSONOutput(WPEFramework::PluginHost::Channel& channel)
        : _exportChannel(channel)
        , _outputoptions(ExtraOutputOptions::ALL)
        , _paused(false)
    {
    }

    bool JSONOutput::IsPaused() const
    {
        return _paused;
    }

    JSONOutput::ExtraOutputOptions JSONOutput::OutputOptions() const
    {
        return _outputoptions;
    }

    void JSONOutput::OutputOptions(const JSONOutput::ExtraOutputOptions outputoptions)
    {
        _outputoptions = outputoptions;
    }

    void JSONOutput::Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message)
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
                data->Filename = info.FileName();
                if ((AsNumber(options) & AsNumber(ExtraOutputOptions::LINENUMBER)) != 0) {
                    data->Linenumber = info.LineNumber();
                }
            }

            if ((AsNumber(options) & AsNumber(ExtraOutputOptions::MODULE)) != 0) {
                data->Module = info.MessageMetaData().Module();
            }

            if ((AsNumber(options) & AsNumber(ExtraOutputOptions::CATEGORY)) != 0) {
                data->Category = info.MessageMetaData().Category();
            }

            string rawMessage;
            message->ToString(rawMessage);
            data->Message = rawMessage;

            HandleJsonMessage(data);
        }
    }

    void JSONOutput::HandleJsonMessage(const Core::ProxyType<Data>& jsondata)
    {
        _exportChannel.Submit(WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement>(jsondata));
    }

    //UDPOutput
    UDPOutput::Channel::Channel(const Core::NodeId& nodeId)
        : Core::SocketDatagram(false, nodeId.Origin(), nodeId, Core::Messaging::MessageUnit::DataSize, 0)
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

        uint16_t actualByteCount = _loaded > maxSendSize ? maxSendSize : _loaded;
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

    void UDPOutput::Channel::Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message)
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

    void UDPOutput::Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message)
    {
        _output.Output(info, message);
    }

    //DIRECTOR
    void MessageDirector::AddOutput(Core::Messaging::MetaData::MessageType type, std::shared_ptr<Messaging::IMessageOutput> output)
    {
        _outputs[type].push_back(std::move(output));
    }

    void MessageDirector::Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message)
    {

        for (const auto& output : _outputs[info.MessageMetaData().Type()]) {
            output->Output(info, message);
        }

        for (const auto& notification : _notifications) {
            string rawMessage;
            message->ToString(rawMessage);

            notification->ReceiveRawMessage(static_cast<Exchange::IMessageControl::MessageType>(info.MessageMetaData().Type()),
                info.MessageMetaData().Category(),
                info.MessageMetaData().Module(),
                info.FileName(),
                info.LineNumber(),
                info.TimeStamp(),
                rawMessage);
        }
    }

    void MessageDirector::RegisterRawMessageNotification(Exchange::IMessageControl::INotification* notification)
    {
        if (notification != nullptr) {
            if (std::find(_notifications.begin(), _notifications.end(), notification) == _notifications.end()) {
                notification->AddRef();
                _notifications.push_back(notification);
            }
        }
    }
    void MessageDirector::UnregisterRawMessageNotification(const Exchange::IMessageControl::INotification* notification)
    {
        auto it = std::find(_notifications.begin(), _notifications.end(), notification);
        if (it != _notifications.end()) {
            (*it)->Release();
            _notifications.erase(it);
        }
    }

}
}