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
#include <interfaces/ITopicExchange.h>
#include <interfaces/json/JTopicExchange.h>

namespace WPEFramework {

namespace Plugin {

    class Linchpin : public PluginHost::IPlugin
                    , public Exchange::ITopicExchange
                    , public PluginHost::JSONRPCSupportsEventStatus {

    public:
        Linchpin(const Linchpin&) = delete;
        Linchpin& operator=(const Linchpin&) = delete;
        Linchpin(Linchpin&&) = delete;
        Linchpin& operator=(Linchpin&&) = delete;

PUSH_WARNING(DISABLE_WARNING_THIS_IN_MEMBER_INITIALIZER_LIST)
        Linchpin()
            : JSONRPCSupportsEventStatus()
            , _adminLock()
        {
        }
POP_WARNING()

        ~Linchpin() override = default;

        // IPlugin methods
        const string Initialize(PluginHost::IShell* service) override;
        void Deinitialize(PluginHost::IShell* service) override;
        string Information() const override  { return { }; }

        // ITopicExchange
        Core::hresult Register(ITopicExchange::INotification* const sink VARIABLE_IS_NOT_USED) override { return Core::ERROR_NONE; }
        Core::hresult Unregister(const ITopicExchange::INotification* const sink VARIABLE_IS_NOT_USED) override { return Core::ERROR_NONE; }

        Core::hresult Connect(const uint32_t operationTimeout VARIABLE_IS_NOT_USED,	const uint32_t retryInterval VARIABLE_IS_NOT_USED, const uint32_t maximumRetryTimeout VARIABLE_IS_NOT_USED) override { return Core::ERROR_NONE; }
        Core::hresult Disconnect() override { return Core::ERROR_NONE; }
        Core::hresult getConnectionStatus() const override { return Core::ERROR_NONE; }

         Core::hresult Register(const string& topicID VARIABLE_IS_NOT_USED, ITopicExchange::IMsgNotification* const sink VARIABLE_IS_NOT_USED) override { return Core::ERROR_NONE; }
         Core::hresult Unregister(const string& topicID VARIABLE_IS_NOT_USED, const ITopicExchange::IMsgNotification* const sink VARIABLE_IS_NOT_USED) override { return Core::ERROR_NONE; }

         Core::hresult getListOfSubscribedTopics(/* use a string iterator here I guess*/) const  override { return Core::ERROR_NONE; }

         Core::hresult Publich(const string& topicID VARIABLE_IS_NOT_USED, const string& payload VARIABLE_IS_NOT_USED, const payloadtype payloadtype VARIABLE_IS_NOT_USED) override { return Core::ERROR_NONE; };
         Core::hresult Fetch(const string& topicID VARIABLE_IS_NOT_USED, string& payload /* @out */ VARIABLE_IS_NOT_USED, payloadtype& payloadtype /* @out */ VARIABLE_IS_NOT_USED)  override { return Core::ERROR_NONE; }
 
        // QueryInterface implementation
        BEGIN_INTERFACE_MAP(Linchpin)
            INTERFACE_ENTRY(PluginHost::IPlugin)
            INTERFACE_ENTRY(Exchange::ITopicExchange)
            INTERFACE_ENTRY(PluginHost::IDispatcher)
        END_INTERFACE_MAP

    private:
        // example of how to call trigger the notifcations:
        void OnConnected() const {
            Exchange::JTopicExchange::Event::Connected(*this);
        }
        void OnDisconnected(const string& reason) const {
            Exchange::JTopicExchange::Event::Disconnected(*this, reason);
        }

        void OnMessage(const string& topicID, const string& payload, const Exchange::ITopicExchange::payloadtype& payloadtype) const {
            Exchange::JTopicExchange::Event::Message(*this, topicID, payload, payloadtype);
        }

    // status listeners:
        void OnMessageSubscribed(const string& topicID VARIABLE_IS_NOT_USED) {
            // do what you need to do:
            // - create connections
            // subscribe to this topics on a connection
            // keep a list of registered clients... (not advised, see interface)
        }
        void OnMessageUnsubscribed(const string& topicID VARIABLE_IS_NOT_USED) {
            // do what you need to do:
            // - close connections
            // unsubscribe to this topics on a connection
            // cleanup the list of registered clients... (not advised, see interface)
        }

    private:
        mutable Core::CriticalSection _adminLock;
    }; // class Linchpin

} // namespace Plugin

} // namespace WPEFramework
