/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 Metrological
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
#include <interfaces/IRustBridge.h>

namespace WPEFramework {
    namespace Plugin {

        class RustBridge :
            public PluginHost::IPluginExtended,
            public PluginHost::JSONRPC
        {
        private:
            enum class state {
                STATE_INCORRECT_HANDLER,
                STATE_INCORRECT_VERSION,
                STATE_UNKNOWN_METHOD,
                STATE_REGISTRATION,
                STATE_UNREGISTRATION,
                STATE_EXISTS,
                STATE_NONE_EXISTING,
                STATE_CUSTOM
            };
            class BridgeCallback : public Exchange::IRustBridge::ICallback {
            public:
                BridgeCallback() = delete;
                BridgeCallback(BridgeCallback&) = delete;
                BridgeCallback(const BridgeCallback&) = delete;
                BridgeCallback& operator= (const BridgeCallback&) = delete;

                explicit BridgeCallback(RustBridge& parent)
                    : _parent(parent)
                {
                }
                ~BridgeCallback() override = default;

            public:
                // ALLOW RUST -> THUNDER (Invoke and Event)
                // The synchronous Invoke from a JSONRPC is coming from the RUST world,
                // synchronously handle this in the Thunder world.
                void Invoke(const string& context, const string& method, const string& parameters, string& response /* @out */, uint32_t& result /* @out */) override {
                   _parent.RustInvoke(context, method, parameters, response, result);
                }

                // Allow RUST to send an event to the interested subscribers in the Thunder
                // world.
                void Event(const string& event, const string& parameters) override {
                    _parent.RustEvent(event, parameters);
                }

                void Response(const uint32_t id, const string& response, const uint32_t error) override {
                    _parent.RustResponse(id, response, error);
                }

                BEGIN_INTERFACE_MAP(BridgeCallback)
                    INTERFACE_ENTRY(Exchange::IRustBridge::ICallback)
                END_INTERFACE_MAP

            private:
                RustBridge& _parent;
            };
            class Notification : public RPC::IRemoteConnection::INotification {
            public:
                Notification() = delete;
                Notification(Notification&&) = delete;
                Notification(const Notification&) = delete;
                Notification& operator= (const Notification&) = delete;

                explicit Notification(RustBridge& parent)
                    : _parent(parent)
                {
                }
                ~Notification() override = default;

            public:
                void Activated(RPC::IRemoteConnection* /* connection */) override
                {
                }
                void Deactivated(RPC::IRemoteConnection* connectionId) override
                {
                    _parent.Deactivated(connectionId);
                }

                BEGIN_INTERFACE_MAP(Notification)
                    INTERFACE_ENTRY(RPC::IRemoteConnection::INotification)
                END_INTERFACE_MAP

            private:
                RustBridge& _parent;
            };
            class Request {
            public:
                Request() = delete;
                Request(Request&&) = delete;
                Request(const Request&) = delete;
                Request& operator=(const Request&) = delete;

                Request(IDispatcher::ICallback* callback, const uint32_t channelId, const uint32_t sequenceId, const Core::Time& timeOut)
                    : _callback(callback)
                    , _channelId(channelId)
                    , _sequenceId(sequenceId)
                    , _issued(timeOut) {
                    _callback->AddRef();
                }
                ~Request() {
                    ASSERT(_callback != nullptr);
                    _callback->Release();
                    _callback = nullptr;
                }

                bool operator== (const IDispatcher::ICallback* callback) const {
                    return (_callback == callback);
                }
                bool operator!= (const IDispatcher::ICallback* callback) const {
                    return (!operator==(callback));
                }

            public:
                IDispatcher::ICallback* Callback() {
                    _callback->AddRef();
                    return (_callback);
                }
                uint32_t ChannelId() const {
                    return (_channelId);
                }
                uint32_t SequenceId() const {
                    return (_sequenceId);
                }
                const Core::Time& Issued() const {
                    return (_issued);
                }

            private:
                IDispatcher::ICallback* _callback;
                uint32_t _channelId;
                uint32_t _sequenceId;
                Core::Time _issued;
            };

            using PendingMap = std::map<uint32_t, Request>;

        public:
            class Config : public Core::JSON::Container {
            public:
                Config(Config&&) = delete;
                Config(const Config&) = delete;
                Config& operator= (const Config&) = delete;

                Config()
                    : Core::JSON::Container()
                    , TimeOut(3000)
                {
                    Add(_T("timeout"), &TimeOut);
                }
                ~Config() override = default;

            public:
                Core::JSON::DecUInt16 TimeOut;
            };

        public:
            RustBridge(const RustBridge&) = delete;
            RustBridge& operator=(const RustBridge&) = delete;

            PUSH_WARNING(DISABLE_WARNING_THIS_IN_MEMBER_INITIALIZER_LIST);
                RustBridge()
                : _adminLock()
                , _skipURL(0)
                , _service(nullptr)
                , _callsign()
                , _pendingRequests()
                , _javascriptService(0)
                , _sequenceId(1)
                , _timeOut(0)
                , _connectionId(0)
                , _module(nullptr)
                , _notification(*this)
                , _callback(*this)
                , _cleaner(*this)
            {
            }
            POP_WARNING();
            ~RustBridge() override = default;

            BEGIN_INTERFACE_MAP(RustBridge)
                INTERFACE_ENTRY(PluginHost::IPlugin)
                INTERFACE_ENTRY(PluginHost::IPluginExtended)
                INTERFACE_ENTRY(PluginHost::IDispatcher)
            END_INTERFACE_MAP

        public:
            //   IPlugin methods
            // -------------------------------------------------------------------------------------------------------
            //! ==================================== CALLED ON THREADPOOL THREAD ======================================
            const string Initialize(PluginHost::IShell* service) override;
            //! ==================================== CALLED ON THREADPOOL THREAD ======================================
            void Deinitialize(PluginHost::IShell* service) override;
            //! ==================================== CALLED ON THREADPOOL THREAD ======================================
            string Information() const override;

            // IDispatcher
            // -------------------------------------------------------------------------------------------------------
            //! ==================================== CALLED ON THREADPOOL THREAD ======================================
            Core::hresult Invoke(IDispatcher::ICallback* callback, const uint32_t channelId, const uint32_t id, const string& token, const string& method, const string& parameters, string& response /* @out */) override;
            Core::hresult Revoke(ICallback* callback) override;

            // IPluginExtended
            // -------------------------------------------------------------------------------------------------------
            //! ================================== CALLED ON COMMUNICATION THREAD =====================================
            bool Attach(PluginHost::Channel& channel) override;
            //! ================================== CALLED ON COMMUNICATION THREAD =====================================
            void Detach(PluginHost::Channel& channel) override;

        private:
            friend class Core::ThreadPool::JobType<RustBridge&>;
            void Dispatch();
            bool InternalMessage(const string& message, const string& parameters);

        private:
            void Deactivated(RPC::IRemoteConnection* connection);
            void RustInvoke(const string& context, const string& method, const string& parmeters, string& response, uint32_t& result);
            void RustEvent(const string & event, const string & parmeters);
            void RustResponse(const uint32_t id, const string& response, const uint32_t error);

        private:
            Core::CriticalSection _adminLock;
            uint8_t _skipURL;
            PluginHost::IShell* _service;
            string _callsign;
            PendingMap _pendingRequests;
            uint32_t _javascriptService;
            uint32_t _sequenceId;
            uint32_t _timeOut;
            uint32_t _connectionId;
            Exchange::IRustBridge* _module;
            Core::Sink<Notification> _notification;
            Core::Sink< BridgeCallback> _callback;
            Core::WorkerPool::JobType<RustBridge&> _cleaner;
        };

    } // namespace Plugin
} // namespace WPEFramework
