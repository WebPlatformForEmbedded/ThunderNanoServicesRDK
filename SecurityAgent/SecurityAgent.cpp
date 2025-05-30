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
 
#include "SecurityAgent.h"
#include "SecurityContext.h"
#include "TokenFactory.h"

namespace Thunder {
ENUM_CONVERSION_BEGIN(Plugin::SecurityAgent::tokentype)
    { Plugin::SecurityAgent::DAC, _TXT("dac") },
ENUM_CONVERSION_END(Plugin::SecurityAgent::tokentype)

namespace Plugin {

    namespace {

        static Metadata<SecurityAgent> metadata(
            // Version
            1, 0, 0,
            // Preconditions
            {},
            // Terminations
            {},
            // Controls
            { subsystem::SECURITY } 
        );
    }

    static Core::ProxyPoolType<Web::TextBody> textFactory(1);

    class SecurityCallsign : public PluginHost::ISubSystem::ISecurity {
    public:
        SecurityCallsign() = delete;
        SecurityCallsign(const SecurityCallsign&) = delete;
        SecurityCallsign& operator=(const SecurityCallsign&) = delete;

        SecurityCallsign(const string callsign)
            : _callsign(callsign)
        {
        }
        virtual ~SecurityCallsign()
        {
        }

    public:
        // Security information
        virtual string Callsign() const
        {
            return (_callsign);
        }

    private:
        BEGIN_INTERFACE_MAP(SecurityCallsign)
        INTERFACE_ENTRY(PluginHost::ISubSystem::ISecurity)
        END_INTERFACE_MAP

    private:
        const string _callsign;
    };

    SecurityAgent::SecurityAgent()
        : _acl()
        , _dispatcher(nullptr)
        , _engine()
        , _testtoken()
    {
    }

    /* virtual */ SecurityAgent::~SecurityAgent()
    {
    }

    /* virtual */ const string SecurityAgent::Initialize(PluginHost::IShell* service)
    {
        string message;
        string webPrefix = service->WebPrefix();
        string callsign = service->Callsign();

        _skipURL = static_cast<uint8_t>(webPrefix.length());
        _servicePrefix = webPrefix.substr(0, webPrefix.find(callsign));

        Config config;
        config.FromString(service->ConfigLine());
        if (config.TestToken.IsSet()) {
            _testtoken = config.TestToken.Value();
        }
        Core::File aclFile(service->PersistentPath() + config.ACL.Value());
        PluginHost::ISubSystem* subSystem = service->SubSystems();

        if (aclFile.Exists() == false) {
            aclFile = service->DataPath() + config.ACL.Value();
        }

        TRACE(Security, (_T("SecurityAgent: Reading acl file %s"), aclFile.Name().c_str()));

        if ((aclFile.Exists() == true) && (aclFile.Open(true) == true)) {

            if (_acl.Load(aclFile) == Core::ERROR_INCOMPLETE_CONFIG) {
                AccessControlList::Iterator index(_acl.Unreferenced());
                while (index.Next()) {
                    SYSLOG(Logging::Startup, (_T("Role: %s not referenced"), index.Current().c_str()));
                }
                index = _acl.Undefined();
                while (index.Next()) {
                    SYSLOG(Logging::Startup, (_T("Role: %s is undefined"), index.Current().c_str()));
                }
            }
        }

        _dacDir = config.DAC.Value();
        if (!_dacDir.empty()) {
            _dacDirCallback = Core::ProxyType<DirectoryCallback>::Create(_dacDir, _dac);
            _dacDirCallback->Updated();

            Core::Directory(_dacDir.c_str()).CreatePath();
            Core::FileSystemMonitor::Instance().Register(&(*_dacDirCallback), _dacDir);
        }

        ASSERT(_dispatcher == nullptr);
        ASSERT(subSystem != nullptr);

        string connector = config.Connector.Value();

        if (connector.empty() == true) {
            connector = service->VolatilePath() + _T("token");
        }

        TRACE(Security, (_T("SecurityAgent TokenDispatcher connector path %s"),connector.c_str()));

        _engine = Core::ProxyType<RPC::InvokeServer>::Create(&Core::IWorkerPool::Instance());
        _dispatcher.reset(new TokenDispatcher(Core::NodeId(connector.c_str()), service->ProxyStubPath(), this, _engine));

        if (_dispatcher != nullptr) {

            if (_dispatcher->IsListening() == false) {
                _dispatcher.reset(nullptr);
                _engine.Release();
            } else {
                if (subSystem != nullptr) {
                    Core::SystemInfo::SetEnvironment(_T("SECURITYAGENT_PATH"), _dispatcher->Connector().c_str(), true);
                    Core::SinkType<SecurityCallsign> information(callsign);

                    if (subSystem->IsActive(PluginHost::ISubSystem::SECURITY) != false) {
                        SYSLOG(Logging::Startup, (_T("Security is not defined as External !!")));
                    }

                    subSystem->Set(PluginHost::ISubSystem::SECURITY, &information);
                    subSystem->Release();
                }
            }

            Exchange::JSecurityAgent::Register(*this, this);
        }
        else {
                message = _T("SecurityAgent failed to create TokenDispatcher");
        }

        // On success return empty, to indicate there is no error text.
        return message;
    }

    /* virtual */ void SecurityAgent::Deinitialize(PluginHost::IShell* service VARIABLE_IS_NOT_USED)
    {
        Exchange::JSecurityAgent::Unregister(*this);

        _acl.Clear();
        _dispatcher.reset(nullptr);

        if (_engine.IsValid()) {
            _engine.Release();
        }

        if (_dacDirCallback.IsValid()) {
            Core::FileSystemMonitor::Instance().Unregister(&(*_dacDirCallback), _dacDir);
            _dacDirCallback.Release();
        }
        _dac.Clear();
    }

    /* virtual */ string SecurityAgent::Information() const
    {
        // No additional info to report.
        return (string());
    }

    /* virtual */ uint32_t SecurityAgent::CreateToken(const uint16_t length, const uint8_t buffer[], string& token)
    {
        TRACE(Security, (_T("Creating Token for %.*s"), length, buffer));

        // Generate the token from the buffer coming in...
        auto newToken = JWTFactory::Instance().Element();

        return (newToken->Encode(token, length, buffer) > 0 ? Core::ERROR_NONE : Core::ERROR_UNAVAILABLE);
    }

    /* virtual */ PluginHost::ISecurity* SecurityAgent::Officer(const string& token)
    {
        PluginHost::ISecurity* result = nullptr;

        if (token.empty() == false) {
            if (token != _testtoken) {

                auto webToken = JWTFactory::Instance().Element();
                ASSERT(webToken != nullptr);
                uint16_t load = webToken->PayloadLength(token);

                // Validate the token
                if (load != static_cast<uint16_t>(~0)) {
                    // It is potentially a valid token, extract the payload.
                    uint8_t* payload = reinterpret_cast<uint8_t*>(ALLOCA(load));

                    load = webToken->Decode(token, load, payload);

                    if (load != static_cast<uint16_t>(~0)) {
                        // Seems like we extracted a valid payload, time to create an security context
                        Payload payloadJson;
                        payloadJson.FromString(string(reinterpret_cast<const TCHAR*>(payload), load));

                        if (payloadJson.Type.IsSet() && (payloadJson.Type == tokentype::DAC)) {
                            result = Core::Service<SecurityContext>::Create<SecurityContext>(&_dac, load, payload, _servicePrefix);
                        } else {
                            result = Core::Service<SecurityContext>::Create<SecurityContext>(&_acl, load, payload, _servicePrefix);
                        }
                    }
                }
            }
#ifdef SECURITY_TESTING_MODE
            else {
                result = Core::ServiceType<SecurityContext>::Create<SecurityContext>(&_acl, static_cast<uint16_t>(sizeof(SecurityAgent::TestTokenContent) - 1), reinterpret_cast<const uint8_t*>(SecurityAgent::TestTokenContent), _servicePrefix);
            }
#endif
        }
        return (result);
    }

    /* virtual */ void SecurityAgent::Inbound(Web::Request& request)
    {
        request.Body(textFactory.Element());
    }

    /* virtual */ Core::ProxyType<Web::Response> SecurityAgent::Process(const Web::Request& request)
    {
        Core::ProxyType<Web::Response> result(PluginHost::IFactories::Instance().Response());
        Core::TextSegmentIterator index(Core::TextFragment(request.Path, _skipURL, static_cast<uint32_t>(request.Path.length() - _skipURL)), false, '/');

        result->ErrorCode = Web::STATUS_BAD_REQUEST;
        result->Message = "Unknown error";

        index.Next();

        if (index.Next() == true) {
            // We might be receiving a plugin download request.
#ifdef SECURITY_TESTING_MODE
            if ((request.Verb == Web::Request::HTTP_PUT) && (request.HasBody() == true)) {
                if (index.Current() == _T("Token")) {
                    Core::ProxyType<const Web::TextBody> data(request.Body<Web::TextBody>());

                    if (data.IsValid() == true) {
                        Core::ProxyType<Web::TextBody> token = textFactory.Element();
                        const string& byteBag(static_cast<const string&>(*data));
                        uint32_t code = CreateToken(static_cast<uint16_t>(byteBag.length()), reinterpret_cast<const uint8_t*>(byteBag.c_str()), static_cast<string&>(*token));

                        if (code == Core::ERROR_NONE) {

                            result->Body(token);
                            result->ContentType = Web::MIMETypes::MIME_TEXT;
                            result->ErrorCode = Web::STATUS_OK;
                            result->Message = "Ok";
                        }
                    }
                }
            } else
#endif

            if ( (request.Verb == Web::Request::HTTP_GET) && (index.Current() == _T("Valid")) ) {
                result->ErrorCode = Web::STATUS_FORBIDDEN;
                result->Message = _T("Missing token");

                if (request.WebToken.IsSet()) {
                    auto webToken = JWTFactory::Instance().Element();
                    ASSERT(webToken != nullptr);
                    const string& token = request.WebToken.Value().Token();
                    uint16_t load = webToken->PayloadLength(token);

                    // Validate the token
                    if (load != static_cast<uint16_t>(~0)) {
                        // It is potentially a valid token, extract the payload.
                        uint8_t* payload = reinterpret_cast<uint8_t*>(ALLOCA(load));

                        load = webToken->Decode(token, load, payload);

                        if (load == static_cast<uint16_t>(~0)) {
                            result->ErrorCode = Web::STATUS_FORBIDDEN;
                            result->Message = _T("Invalid token");
                        } else {
                            result->ErrorCode = Web::STATUS_OK;
                            result->Message = _T("Valid token");
                            TRACE(Security, (_T("Token contents: %s"), reinterpret_cast<const TCHAR*>(payload)));
                        }
                    }
                }
            }
        }
        return (result);
    }

    Core::hresult SecurityAgent::Validate(const string& token, bool& valid) const
    {
        valid = false;

        auto webToken = JWTFactory::Instance().Element();

        ASSERT(webToken != nullptr);

        uint16_t load = webToken->PayloadLength(token);

        // Validate the token
        if (load != static_cast<uint16_t>(~0)) {
            // It is potentially a valid token, extract the payload.
            uint8_t* payload = reinterpret_cast<uint8_t*>(ALLOCA(load));

            load = webToken->Decode(token, load, payload);

            if (load != static_cast<uint16_t>(~0)) {
                valid = true;
            }
        }

        return (Core::ERROR_NONE);
    }

} // namespace Plugin
} // namespace Thunder
