#include "MessageOutput.h"

namespace WPEFramework {
namespace Messaging {

    void ConsoleOutput::Output(const string& message)
    {
        std::cout << message;
    }

    void SyslogOutput::Output(const string& message)
    {
#ifndef __WINDOWS__
        syslog(LOG_NOTICE, _T("%s"), message.c_str());
#else
        printf(_T("%s"), message.c_str());
#endif
    }

    FileOutput::FileOutput(const string& filepath)
        : _file(filepath)
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

    void FileOutput::Output(const string& message)
    {
        if (_file.IsOpen()) {
            _file.Write(reinterpret_cast<const uint8_t*>(message.c_str()), message.length());
        }
    }

    //DIRECTOR
    MessageDirector::MessageDirector()
        : _abbreviate(false)
    {
        _formatters[Core::Messaging::MetaData::MessageType::TRACING].reset(new Trace::Formatter());
    }

    void MessageDirector::AbbreviateMessages(bool abbreviate)
    {
        _abbreviate = abbreviate;
    }
    void MessageDirector::AddOutput(std::unique_ptr<Messaging::IMessageOutput> output)
    {
        _outputs.push_back(std::move(output));
    }

    void MessageDirector::Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message)
    {
        auto formatter = _formatters.find(info.MessageMetaData().Type());
        auto preparedMessage = formatter->second->Prepare(_abbreviate, info, message);

        for (const auto& output : _outputs) {
            output->Output(preparedMessage);
        }
    }

}
}