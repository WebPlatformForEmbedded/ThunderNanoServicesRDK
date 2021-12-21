#pragma once
#include "Module.h"
namespace WPEFramework {

struct IMessageAssembler {
    virtual ~IMessageAssembler() = default;
    virtual string Prepare(const bool abbreviateMessage, const Core::MessageInformation& info, const Core::IMessageEvent* message) const = 0;
};

struct IMessageOutput {
    virtual ~IMessageOutput() = default;
    virtual void Output(const string& message) = 0;
};

class TraceFormatter : public IMessageAssembler {
public:
    TraceFormatter()
    {
    }
    string Prepare(const bool abbreviateMessage, const Core::MessageInformation& info, const Core::IMessageEvent* message) const override
    {
        _output.str("");
        _output.clear();

        message->ToString(_deserializedMessage);

        string time(Core::Time::Now().ToRFC1123(true));
        _output << '[' << time.c_str() << "]:[" << Core::FileNameOnly(info.FileName().c_str()) << ':' << info.LineNumber() << "] "
                << info.MetaData().Category() << ": " << _deserializedMessage << std::endl;

        return _output.str();
    }

private:
    mutable string _deserializedMessage;
    mutable std::ostringstream _output;
    bool _abbreviated;
};

class ConsoleOutput : public IMessageOutput {
public:
    ConsoleOutput() = default;
    ~ConsoleOutput() override = default;
    ConsoleOutput(const ConsoleOutput&) = delete;
    ConsoleOutput& operator=(const ConsoleOutput&) = delete;

    void Output(const string& message) override;
};

class SyslogOutput : public IMessageOutput {
public:
    SyslogOutput() = default;
    ~SyslogOutput() override = default;
    SyslogOutput(const SyslogOutput&) = delete;
    SyslogOutput& operator=(const SyslogOutput&) = delete;

    void Output(const string& message) override;
};

class FileOutput : public IMessageOutput {
public:
    explicit FileOutput(const string& filepath);
    ~FileOutput() override;
    FileOutput(const FileOutput&) = delete;
    FileOutput& operator=(const FileOutput&) = delete;

    void Output(const string& message) override;

private:
    Core::File _file;
};

class MessageDirector {
public:
    MessageDirector();
    ~MessageDirector() = default;
    MessageDirector(const MessageDirector&) = delete;
    MessageDirector& operator=(const MessageDirector&) = delete;

    void AbbreviateMessages(bool abbreviate);
    void AddOutput(std::unique_ptr<IMessageOutput> output);
    void Output(const Core::MessageInformation& info, const Core::IMessageEvent* message);

private:
    std::unordered_map<Core::MessageMetaData::MessageType, std::unique_ptr<IMessageAssembler>> _formatters;
    std::list<std::unique_ptr<IMessageOutput>> _outputs;
    bool _abbreviate;
};
}