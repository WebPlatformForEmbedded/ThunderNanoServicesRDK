#include "MessageOutput.h"

namespace WPEFramework {
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
    _formatters[Core::MessageMetaData::MessageType::TRACING].reset(new TraceFormatter());
}

void MessageDirector::AbbreviateMessages(bool abbreviate)
{
    _abbreviate = abbreviate;
}
void MessageDirector::AddOutput(std::unique_ptr<IMessageOutput> output)
{
    _outputs.push_back(std::move(output));
}

void MessageDirector::Output(const Core::MessageInformation& info, const Core::IMessageEvent* message)
{
    auto formatter = _formatters.find(info.MetaData().Type());
    auto preparedMessage = formatter->second->Prepare(_abbreviate, info, message);

    for (const auto& output : _outputs) {
        output->Output(preparedMessage);
    }
}

}