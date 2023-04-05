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
#include "AccessControlList.h"

#include <securityagent/IPCSecurityToken.h>
#include <interfaces/json/JsonData_SecurityAgent.h>

namespace WPEFramework {
namespace Plugin {

    class SecurityAgent : public PluginHost::IAuthenticate,
                            public PluginHost::IPlugin,
                            public PluginHost::JSONRPC,
                            public PluginHost::IWeb {
    private:
        class TokenDispatcher : public RPC::Communicator {
        public:
            TokenDispatcher() = delete;
            TokenDispatcher(const TokenDispatcher&) = delete;
            TokenDispatcher& operator=(const TokenDispatcher&) = delete;

            TokenDispatcher(
                const Core::NodeId& source,
                const std::string& proxyStubPath,
                PluginHost::IAuthenticate* parentInterface,
                const Core::ProxyType<RPC::InvokeServer>& engine
                )
                : RPC::Communicator(source, proxyStubPath, Core::ProxyType<Core::IIPCServer>(engine))
                , _parentInterface(parentInterface)
            {   
                if(_parentInterface != nullptr){
                    _parentInterface->AddRef();
                }
                engine->Announcements(Announcement());
                Open(Core::infinite);
            }
            ~TokenDispatcher() override
            {
                if(_parentInterface != nullptr){
                    _parentInterface->Release();
                }

                Close(Core::infinite);
            }

        private:
            void* Acquire(const string&, const uint32_t interfaceId, const uint32_t versionId) override
            {
                void* result = nullptr;

                if (((versionId == 1) || (versionId == static_cast<uint32_t>(~0))) && ((interfaceId == PluginHost::IAuthenticate::ID) || (interfaceId == Core::IUnknown::ID))) {
                    
                    _parentInterface->AddRef();


                    TRACE(Security, ("SecurityAgent interface(IAuthenticate) acquired => %p", this));
                    result = _parentInterface;
                }
                return (result);
            }

        private:
            PluginHost::IAuthenticate* _parentInterface;
        };

        class Config : public Core::JSON::Container {
        private:
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;

        public:
            Config()
                : Core::JSON::Container()
                , ACL(_T("acl.json"))
                , Connector()
                , TestToken()
            {
                Add(_T("acl"), &ACL);
                Add(_T("connector"), &Connector);
                Add(_T("testtoken"), &TestToken);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::String ACL;
            Core::JSON::String Connector;
            Core::JSON::String TestToken;
        };

    public:
        SecurityAgent(const SecurityAgent&) = delete;
        SecurityAgent& operator=(const SecurityAgent&) = delete;

        SecurityAgent();
        virtual ~SecurityAgent();

        // Build QueryInterface implementation, specifying all possible interfaces to be returned.
        BEGIN_INTERFACE_MAP(SecurityAgent)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IWeb)
        INTERFACE_ENTRY(PluginHost::IAuthenticate)
        INTERFACE_ENTRY(PluginHost::IDispatcher)
        END_INTERFACE_MAP

    public:
        //   IPlugin methods
        // -------------------------------------------------------------------------------------------------------
        virtual const string Initialize(PluginHost::IShell* service) override;
        virtual void Deinitialize(PluginHost::IShell* service) override;
        virtual string Information() const override;

        //   IAuthenticate methods
        // -------------------------------------------------------------------------------------------------------
        virtual uint32_t CreateToken(const uint16_t length, const uint8_t buffer[], string& token);
        virtual PluginHost::ISecurity* Officer(const string& token);

        //   IWeb methods
        // -------------------------------------------------------------------------------------------------------
        //! Whenever a request is received, it might carry some additional data in the body. This method allows
        //! the plugin to attach a deserializable data object (ref counted) to be loaded with any potential found
        //! in the body of the request.
        //! @}
        virtual void Inbound(Web::Request& request);

        //! @{
        //! ==================================== CALLED ON THREADPOOL THREAD ======================================
        //! If everything is received correctly, the request is passed to us, on a thread from the thread pool, to
        //! do our thing and to return the result in the response object. Here the actual specific module work,
        //! based on a a request is handled.
        //! @}
        virtual Core::ProxyType<Web::Response> Process(const Web::Request& request);

    private:
        //   JsonRPC methods
        // -------------------------------------------------------------------------------------------------------
        void RegisterAll();
        void UnregisterAll();
#ifdef SECURITY_TESTING_MODE
        uint32_t endpoint_createtoken(const JsonData::SecurityAgent::CreatetokenParamsData& params, JsonData::SecurityAgent::CreatetokenResultInfo& response);
#endif // DEBUG
        uint32_t endpoint_validate(const JsonData::SecurityAgent::CreatetokenResultInfo& params, JsonData::SecurityAgent::ValidateResultData& response);

    private:
        AccessControlList _acl;
        uint8_t _skipURL;
        std::unique_ptr<TokenDispatcher> _dispatcher; 
        Core::ProxyType<RPC::InvokeServer> _engine;
        string _testtoken;
        string _servicePrefix;

#ifdef SECURITY_TESTING_MODE
        static constexpr const TCHAR* TestTokenContent = _T(R"--({ "url": "https://test.url.com", "user":"Test" })--");
#endif // DEBUG
    };

} // namespace Plugin
} // namespace WPEFramework
