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
#include <typeindex>

namespace WPEFramework {
namespace Plugin {
    class WarningReportingControl : public PluginHost::IPluginExtended,
                                    public PluginHost::IWebSocket {

    public:
        enum state {
            ENABLED,
            DISABLED,
            TRISTATED
        };

    private:
        WarningReportingControl(const WarningReportingControl&) = delete;
        WarningReportingControl& operator=(const WarningReportingControl&) = delete;

        struct WarningInformation {
            std::unique_ptr<WarningReporting::IWarningEvent> event;
            uint32_t lineNumber;
            std::string filename;
            std::string category;
            std::string identifier;
        };

        class Observer : public Core::Thread, public RPC::IRemoteConnection::INotification {
        private:
            Observer() = delete;
            Observer(const Observer&) = delete;
            Observer& operator=(const Observer&) = delete;

        public:
            class Source : public Core::CyclicBuffer {
            public:
                enum state {
                    EMPTY,
                    LOADED,
                    FAILURE
                };

            public:
                Source() = default;
                Source(const Source&) = default;
                Source& operator=(const Source&) = default;

                Source(const string& warningsPath, RPC::IRemoteConnection* connection)
                    : Core::CyclicBuffer(SourceName(warningsPath, connection), Core::File::USER_WRITE | Core::File::USER_READ | Core::File::SHAREABLE, 0, true)
                    , _connection(connection)
                    , _state(EMPTY)
                {
                    if (_connection != nullptr) {
                        _connection->AddRef();
                    }
                }
                ~Source()
                {
                    if (_connection != nullptr) {
                        _connection->Release();
                        _connection = nullptr;
                    }
                }

            public:
                uint32_t Id() const
                {
                    return (_connection != nullptr ? _connection->Id() : 0);
                }

                state Load(WarningReporting::WarningReportingUnit& warningReportingUnit)
                {
                    uint32_t length;
                    bool available = Core::CyclicBuffer::IsValid();

                    if (available == false) {
                        available = Core::CyclicBuffer::Validate();
                    }

                    // Traces will be commited in one go, First reserve, then write. So if there is a length (2 bytes)
                    // The full trace has to be available as well.
                    if ((available == true) && (_state == EMPTY) && ((length = Read(_warningsBuffer, sizeof(_warningsBuffer))) != 0)) {

                        if (length < 2) {
                            // Didn't even get enough data to read entry size. This is impossible, fallback to failure.
                            TRACE(Trace::Error, (_T("Inconsistent trace dump. Need to flush. %d"), length));
                            _state = FAILURE;
                        } else {

                            uint16_t requiredLength = (_warningsBuffer[1] << 8) | _warningsBuffer[0];

                            if (requiredLength != length) {
                                // Something went wrong, didn't read a full entry.
                                _state = FAILURE;
                            } else {

                                //skip length
                                uint32_t offset = 2;

                                ::memcpy(&_timeStamp, &(_warningsBuffer[offset]), sizeof(uint32_t));
                                offset += sizeof(_timeStamp);

                                ::memcpy(&information.lineNumber, &(_warningsBuffer[offset]), sizeof(uint32_t));
                                offset += sizeof(information.lineNumber);

                                information.filename = reinterpret_cast<const char*>(_warningsBuffer + offset);
                                offset += information.filename.length() + 1; //include null terminator

                                information.category = reinterpret_cast<const char*>(_warningsBuffer + offset);
                                offset += information.category.length() + 1;

                                information.identifier = reinterpret_cast<const char*>(_warningsBuffer + offset);
                                offset += information.identifier.length() + 1;

                                ASSERT(length >= offset);
                                //rest of the information is data to be deserialized

                                information.event.reset(warningReportingUnit.Clone(information.category));

                                if (information.event != nullptr) {
                                    auto length = requiredLength - offset;
                                    information.event->Deserialize(&_warningsBuffer[offset], length);
                                }

                                // Entries are read in whole, so we are done.
                                _state = LOADED;
                            }
                        }
                    }
                    return _state;
                }

                inline uint64_t Timestamp() const
                {
                    return _timeStamp;
                }

                void Flush()
                {
                    _state = EMPTY;
                    Core::CyclicBuffer::Flush();
                }
                void Clear()
                {
                    _state = EMPTY;
                }

                const WarningInformation& warningInformation() const
                {
                    return information;
                }

            private:
                static string SourceName(const string& pathName, RPC::IRemoteConnection* connection);

                uint32_t GetReadSize(Core::CyclicBuffer::Cursor& cursor) override
                {
                    // Just read one entry.
                    uint16_t entrySize = 0;
                    cursor.Peek(entrySize);
                    return entrySize;
                }

                RPC::IRemoteConnection* _connection;
                WarningInformation information;

                state _state;
                uint64_t _timeStamp;
                uint8_t _warningsBuffer[WarningReporting::CyclicBufferSize];
            };

        public:
            Observer(WarningReportingControl& parent)
                : Thread(Core::Thread::DefaultStackSize(), _T("WarningReportingWorker"))
                , _maxUint64value(std::numeric_limits<uint64_t>::max())
                , _parent(parent)
                , _refcount(0)
                , _warningReportingUnit(WarningReporting::WarningReportingUnit::Instance())
            {
            }
            ~Observer()
            {
                ASSERT(_refcount == 0);
                ASSERT(_buffers.size() == 0);
                _warningReportingUnit.Relinquish();
                Wait(Thread::BLOCKED | Thread::STOPPED | Thread::STOPPING, Core::infinite);
            }

        public:
            void Reevaluate()
            {
                _warningReportingUnit.Announce();
            }
            void Start()
            {
                _buffers.emplace(std::piecewise_construct,
                    std::forward_as_tuple(0),
                    std::forward_as_tuple(_parent.WarningsPath(), nullptr));

                _warningReportingUnit.Announce();
                Thread::Run();
            }
            void Stop()
            {
                Block();

                _warningReportingUnit.Announce();

                Wait(Thread::BLOCKED | Thread::STOPPED | Thread::STOPPING, Core::infinite);

                _adminLock.Lock();

                _buffers.clear();

                _adminLock.Unlock();
            }
            void Activated(RPC::IRemoteConnection* connection) override
            {
                ASSERT(connection != nullptr);
                _adminLock.Lock();

                if (connection != nullptr) {

                    ASSERT(_buffers.find(connection->Id()) == _buffers.end());

                    if (_buffers.find(connection->Id()) == _buffers.end()) {

                        // By definition, get the buffer file from WPEFramework (local source)
                        _buffers.emplace(std::piecewise_construct,
                            std::forward_as_tuple(connection->Id()),
                            std::forward_as_tuple(_parent.WarningsPath(), connection));

                        _warningReportingUnit.Announce();
                    }
                }

                _adminLock.Unlock();
            }
            void Deactivated(RPC::IRemoteConnection* connection) override
            {
                _adminLock.Lock();

                auto index = _buffers.find(connection->Id());

                if (index != _buffers.end()) {
                    _buffers.erase(index);
                }

                _adminLock.Unlock();
            }

        private:
            BEGIN_INTERFACE_MAP(Observer)
            INTERFACE_ENTRY(RPC::IRemoteConnection::INotification)
            END_INTERFACE_MAP

            void AddRef() const override
            {
                Core::InterlockedIncrement(_refcount);
            }
            uint32_t Release() const override
            {
                Core::InterlockedDecrement(_refcount);

                return (Core::ERROR_NONE);
            }
            uint32_t Worker() override
            {
                while ((IsRunning() == true) && (_warningReportingUnit.Wait(Core::infinite) == Core::ERROR_NONE)) {
                    // Before we start we reset the flag, if new info is coming in, we will get a retrigger flag.
                    _warningReportingUnit.Acknowledge();
                    std::unordered_map<uint32_t, Source>::iterator selected = _buffers.end();
                    uint64_t timeStamp = _maxUint64value;

                    do {
                        selected = _buffers.end();

                        _adminLock.Lock();

                        timeStamp = _maxUint64value;

                        for (auto buffer = _buffers.begin(); buffer != _buffers.end(); ++buffer) {
                            Source::state state(buffer->second.Load(_warningReportingUnit));

                            if (state == Source::LOADED) {
                                if (buffer->second.Timestamp() < timeStamp) {
                                    timeStamp = buffer->second.Timestamp();

                                    if (state == Source::LOADED) {
                                        selected = buffer;
                                    }
                                }
                            } else if (state == Source::FAILURE) {
                                // Oops this requires recovery, so let's flush
                                buffer->second.Flush();
                            }
                        }

                        if (selected != _buffers.end()) {

                            // Oke, output this entry
                            _parent.Dispatch(selected->second.warningInformation());

                            // Ready to load a new one..
                            selected->second.Clear();
                        } else if (timeStamp != _maxUint64value) {
                            // Looks like we are waiting for a message to be completed.
                            // Give up our slice, so the producer, can produce.
                            std::this_thread::yield();
                        }

                        _adminLock.Unlock();

                    } while ((IsRunning() == true) && (timeStamp != _maxUint64value));
                }

                return (Core::infinite);
            }

        private:
            const uint64_t _maxUint64value;
            Core::CriticalSection _adminLock;
            std::unordered_map<uint32_t, Source> _buffers;
            WarningReporting::WarningReportingUnit& _warningReportingUnit;
            WarningReportingControl& _parent;
            mutable uint32_t _refcount;
        };

    public:
        class Config : public Core::JSON::Container {
        private:
            Config(const Config&);
            Config& operator=(const Config&);

        public:
            Config()
                : Core::JSON::Container()
                , FilePath()
                , Console(false)
                , SysLog(true)
                , Abbreviated(true)
                , MaxExportConnections(1)
            {
                Add(_T("console"), &Console);
                Add(_T("syslog"), &SysLog);
                Add(_T("filepath"), &FilePath);
                Add(_T("abbreviated"), &Abbreviated);
                Add(_T("maxexportconnections"), &MaxExportConnections);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::String FilePath;
            Core::JSON::Boolean Console;
            Core::JSON::Boolean SysLog;
            Core::JSON::Boolean Abbreviated;
            Core::JSON::DecUInt16 MaxExportConnections;
        };

    public:
#ifdef __WINDOWS__
#pragma warning(disable : 4355)
#endif
        WarningReportingControl()
            : _service(nullptr)
            , _outputs()
            , _warningsPath()
            , _observer(*this)
        {
        }
#ifdef __WINDOWS__
#pragma warning(default : 4355)
#endif
        ~WarningReportingControl() override
        {
        }

        BEGIN_INTERFACE_MAP(WarningReportingControl)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IPluginExtended)
        INTERFACE_ENTRY(PluginHost::IWebSocket)
        END_INTERFACE_MAP

    public:
        //  IPlugin methods
        // -------------------------------------------------------------------------------------------------------

        // First time initialization. Whenever a plugin is loaded, it is offered a Service object with relevant
        // information and services for this particular plugin. The Service object contains configuration information that
        // can be used to initialize the plugin correctly. If Initialization succeeds, return nothing (empty string)
        // If there is an error, return a string describing the issue why the initialisation failed.
        // The Service object is *NOT* reference counted, lifetime ends if the plugin is deactivated.
        // The lifetime of the Service object is guaranteed till the deinitialize method is called.
        const string Initialize(PluginHost::IShell* service) override;

        // The plugin is unloaded from WPEFramework. This is call allows the module to notify clients
        // or to persist information if needed. After this call the plugin will unlink from the service path
        // and be deactivated. The Service object is the same as passed in during the Initialize.
        // After theis call, the lifetime of the Service object ends.
        void Deinitialize(PluginHost::IShell* service) override;

        // Returns an interface to a JSON struct that can be used to return specific metadata information with respect
        // to this plugin. This Metadata can be used by the MetData plugin to publish this information to the ouside world.
        string Information() const override;

        //	IPluginExtended methods
        // -------------------------------------------------------------------------------------------------------

        // Whenever a Channel (WebSocket connection) is created to the plugin that will be reported via the Attach.
        // Whenever the channel is closed, it is reported via the detach method.
        bool Attach(PluginHost::Channel& channel) override;
        void Detach(PluginHost::Channel& channel) override;

        //! @{
        //! ================================== CALLED ON COMMUNICATION THREAD =====================================
        //! Whenever a WebSocket is opened with a locator (URL) pointing to this plugin, it is capable of sending
        //! JSON object to us. This method allows the plugin to return a JSON object that will be used to deserialize
        //! the comming content on the communication channel. In case the content does not start with a { or [, the
        //! first keyword deserialized is passed as the identifier.
        //! @}
        Core::ProxyType<Core::JSON::IElement> Inbound(const string& identifier) override;

        //! @{
        //! ==================================== CALLED ON THREADPOOL THREAD ======================================
        //! Once the passed object from the previous method is filled (completed), this method allows it to be handled
        //! and to form an answer on the incoming JSON message(if needed).
        //! @}
        Core::ProxyType<Core::JSON::IElement> Inbound(const uint32_t ID, const Core::ProxyType<Core::JSON::IElement>& element) override;

    private:
        void Dispatch(const WarningInformation& information);

        inline const string& WarningsPath() const
        {
            return (_warningsPath);
        }

    private:
        //I will need to quickly find WebSocketExporter class
        using WarningsMediaContainer = std::unordered_map<std::type_index, std::unique_ptr<WarningReporting::IWarningReportingMedia>>;

        PluginHost::IShell* _service;
        Config _config;
        WarningsMediaContainer _outputs;
        string _warningsPath;
        Observer _observer;
    };
}
}
