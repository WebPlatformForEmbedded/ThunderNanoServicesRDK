#pragma once
#include "Module.h"
namespace WPEFramework {
namespace Messaging {

    class ConsoleOutput : public Messaging::IMessageOutput {
    public:
        ConsoleOutput() = default;
        ~ConsoleOutput() override = default;
        ConsoleOutput(const ConsoleOutput&) = delete;
        ConsoleOutput& operator=(const ConsoleOutput&) = delete;

        void Output(const string& message) override;
    };

    class SyslogOutput : public Messaging::IMessageOutput {
    public:
        SyslogOutput() = default;
        ~SyslogOutput() override = default;
        SyslogOutput(const SyslogOutput&) = delete;
        SyslogOutput& operator=(const SyslogOutput&) = delete;

        void Output(const string& message) override;
    };

    class FileOutput : public Messaging::IMessageOutput {
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
        void AddOutput(std::unique_ptr<Messaging::IMessageOutput> output);
        void Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message);

    private:
        std::unordered_map<Core::Messaging::MetaData::MessageType, std::unique_ptr<Core::Messaging::IAssembler>> _formatters;
        std::list<std::unique_ptr<IMessageOutput>> _outputs;
        bool _abbreviate;
    };
}
}