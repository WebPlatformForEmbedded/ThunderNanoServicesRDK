#pragma once
#include "Module.h"
namespace WPEFramework {

struct IMessageAssembler {
    virtual ~IMessageAssembler() = default;
    virtual string Prepare(const bool abbreviateMessage, const Core::MessageInformation& info, const Core::IMessageEvent* message) const = 0;
};

struct IMessageOutput {
    virtual ~IMessageOutput() = default;
    virtual void Output(const string& message) const = 0;
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
                << info.MetaData().Category() << ": " << _deserializedMessage;

        return _output.str();
    }

private:
    mutable string _deserializedMessage;
    mutable std::ostringstream _output;
    bool _abbreviated;
};

class ConsoleOutput : public IMessageOutput {
public:
    void Output(const string& message) const override
    {
        std::cout << message << std::endl;
    }
};

class MessageDirector {
public:
    MessageDirector()
    {
        _formatters[Core::MessageMetaData::MessageType::TRACING].reset(new TraceFormatter());
    }
    ~MessageDirector() = default;

    void AbbreviateMessages(bool abbreviate)
    {
        _abbreviate = abbreviate;
    }
    void AddOutput(std::unique_ptr<IMessageOutput> output)
    {
        _outputs.push_back(std::move(output));
    }

    void Output(const Core::MessageInformation& info, const Core::IMessageEvent* message)
    {
        auto formatter = _formatters.find(info.MetaData().Type());
        auto preparedMessage = formatter->second->Prepare(_abbreviate, info, message);

        for (const auto& output : _outputs) {
            output->Output(preparedMessage);
        }
    }

private:
    std::unordered_map<Core::MessageMetaData::MessageType, std::unique_ptr<IMessageAssembler>> _formatters;
    std::list<std::unique_ptr<IMessageOutput>> _outputs;
    bool _abbreviate;
};
}