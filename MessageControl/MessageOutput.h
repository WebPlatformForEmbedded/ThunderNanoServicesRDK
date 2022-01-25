#pragma once
#include "Module.h"
namespace WPEFramework {
namespace Messaging {

    class TextOutput : public Messaging::IMessageOutput {
    public:
        explicit TextOutput(const bool abbreviated);
        ~TextOutput() override = default;

        TextOutput(const TextOutput&) = delete;
        TextOutput& operator=(const TextOutput&) = delete;

    public:
        void Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message) override;

    private:
        virtual void HandleTextMessage(const string& message) = 0;

    private:
        string _deserializedMessage;
        std::ostringstream _output;
        bool _abbreviated;
    };

    class ConsoleOutput : public TextOutput {
    public:
        explicit ConsoleOutput(bool abbreviate)
            : TextOutput(abbreviate)
        {
        }
        ~ConsoleOutput() override = default;
        ConsoleOutput(const ConsoleOutput&) = delete;
        ConsoleOutput& operator=(const ConsoleOutput&) = delete;

        void HandleTextMessage(const string& message) override;
    };

    class SyslogOutput : public TextOutput {
    public:
        explicit SyslogOutput(bool abbreviate)
            : TextOutput(abbreviate)
        {
        }
        ~SyslogOutput() override = default;
        SyslogOutput(const SyslogOutput&) = delete;
        SyslogOutput& operator=(const SyslogOutput&) = delete;

        void HandleTextMessage(const string& message) override;
    };

    class FileOutput : public TextOutput {
    public:
        explicit FileOutput(bool abbreviate, const string& filepath);
        ~FileOutput() override;
        FileOutput(const FileOutput&) = delete;
        FileOutput& operator=(const FileOutput&) = delete;

        void HandleTextMessage(const string& message) override;

    private:
        Core::File _file;
    };

    class EXTERNAL JSONOutput : public IMessageOutput {
    public:
        class Data : public Core::JSON::Container {

        public:
            Data();
            ~Data() override = default;

            Data(const Data&) = delete;
            Data& operator=(const Data&) = delete;

        public:
            Core::JSON::String Time;
            Core::JSON::String Filename;
            Core::JSON::DecUInt32 Linenumber;
            Core::JSON::String Category;
            Core::JSON::String Module;
            Core::JSON::String Message;
        };

    public:
        JSONOutput(const JSONOutput&) = delete;
        JSONOutput& operator=(const JSONOutput&) = delete;

        enum class ExtraOutputOptions {
            ABREVIATED = 0,
            FILENAME = 1,
            LINENUMBER = 3, // selecting LINENUMBER will automatically select FILENAME
            MODULE = 4,
            CATEGORY = 8,
            INCLUDINGDATE = 16,
            ALL = 31
        };

        explicit JSONOutput(WPEFramework::PluginHost::Channel& channel);
        ~JSONOutput() = default;

    public:
        template <typename E>
        static inline auto AsNumber(E t) -> typename std::underlying_type<E>::type
        {
            return static_cast<typename std::underlying_type<E>::type>(t);
        }

        bool IsPaused() const;
        ExtraOutputOptions OutputOptions() const;
        void OutputOptions(const ExtraOutputOptions outputoptions);
        void Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message) override;

        virtual WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement> Process(const WPEFramework::Core::ProxyType<WPEFramework::Core::JSON::IElement>& element) = 0;

    private:
        virtual Core::ProxyType<Data> GetDataContainer() = 0;
        void HandleJsonMessage(const Core::ProxyType<Data>& jsondata);

    protected:
        WPEFramework::PluginHost::Channel& _exportChannel;
        std::atomic<ExtraOutputOptions> _outputoptions;
        bool _paused;
    };

    class MessageDirector {
    public:
        MessageDirector() = default;
        ~MessageDirector() = default;
        MessageDirector(const MessageDirector&) = delete;
        MessageDirector& operator=(const MessageDirector&) = delete;

        void AddOutput(Core::Messaging::MetaData::MessageType type, std::unique_ptr<Messaging::IMessageOutput> output);
        void Output(const Core::Messaging::Information& info, const Core::Messaging::IEvent* message);

        void RegisterRawMessageNotification(Exchange::IMessageControl::INotification* notification);
        void UnregisterRawMessageNotification(const Exchange::IMessageControl::INotification* notification);

    private:
        using Outputs = std::unordered_map<Core::Messaging::MetaData::MessageType, std::list<std::unique_ptr<IMessageOutput>>>;
        Outputs _outputs;
        std::list<Exchange::IMessageControl::INotification*> _notifications;
    };
}
}