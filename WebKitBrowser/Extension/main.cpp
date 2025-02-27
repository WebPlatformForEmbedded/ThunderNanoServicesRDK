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

#include "Module.h"

#include <cstdio>
#include <memory>
#include <syslog.h>

#include <wpe/webkit-web-extension.h>

#include "../BrowserConsoleLog.h"
#include "../Tags.h"

#include "Milestone.h"
#include "NotifyThunder.h"
#include "RequestHeaders.h"
#include "WhiteListedOriginDomainsList.h"

#ifdef ENABLE_SECURITY_AGENT
#include "SecurityAgent.h"
#endif

#if defined(ENABLE_IIDENTIFIER)
#include "IIdentifier.h"
#endif

#if defined(ENABLE_BADGER_BRIDGE)
#include "BridgeObject.h"
#endif

#if defined(ENABLE_AAMP_JSBINDINGS)
#include "AAMPJSBindings.h"
#endif

#if defined(UPDATE_TZ_FROM_FILE)
#include "TimeZoneSupport.h"
#endif

#if defined(ENABLE_TESTING)
#include <TestRunnerJS.h>
#endif

using namespace Thunder;

static Core::NodeId GetConnectionNode()
{
    string nodeName;

    Core::SystemInfo::GetEnvironment(string(_T("COMMUNICATOR_CONNECTOR")), nodeName);

    return (Core::NodeId(nodeName.c_str()));
}

static class PluginHost {
private:
    PluginHost(const PluginHost&) = delete;
    PluginHost& operator=(const PluginHost&) = delete;

public:
    PluginHost()
        : _engine(Core::ProxyType<RPC::InvokeServerType<2, 0, 4>>::Create())
        , _comClient(Core::ProxyType<RPC::CommunicatorClient>::Create(GetConnectionNode(), Core::ProxyType<Core::IIPCServer>(_engine)))
        , _extension(nullptr)
    {
    }
    ~PluginHost()
    {
        TRACE(Trace::Information, (_T("Destructing injected extension stuff!!! [%d]"), __LINE__));
        Deinitialize();
    }

public:
    void Initialize(WebKitWebExtension* extension, const void* userData)
    {
        ASSERT(_comClient.IsValid() == true);
        // We have something to report back, do so...
        uint32_t result = _comClient->Open(RPC::CommunicationTimeOut);
        if (result != Core::ERROR_NONE) {
            TRACE(Trace::Error, (_T("Could not open connection to node %s. Error: %s"), _comClient->Source().RemoteId(), Core::NumberType<uint32_t>(result).Text()));
        } else {
            // Due to the LXC container support all ID's get mapped. For the TraceBuffer, use the host given ID.
            Messaging::MessageUnit::Instance().Open(_comClient->ConnectionId());

            // TRACE_CONTROL(BrowserConsoleLog)::Enable(true);
        }

        _extension = WEBKIT_WEB_EXTENSION(g_object_ref(extension));
        gboolean _logToSystemConsoleEnabled = FALSE;

        const char *uid;
        const char *whitelist;

        g_variant_get((GVariant*) userData, "(&sm&sb)", &uid, &whitelist, &_logToSystemConsoleEnabled);

        Core::SystemInfo::GetEnvironment(string(_T("CLIENT_IDENTIFIER")), _consoleLogPrefix);
        _consoleLogPrefix = _consoleLogPrefix.substr(0, _consoleLogPrefix.find(','));

        g_signal_connect(
          webkit_script_world_get_default(),
          "window-object-cleared",
          G_CALLBACK(windowObjectClearedCallback),
          this);

        g_signal_connect(
          extension,
          "page-created",
          G_CALLBACK(pageCreatedCallback),
          this);

        if (whitelist != nullptr) {
            auto list = WebKit::WhiteListedOriginDomainsList::Parse(whitelist);
            if (list) {
              list->AddWhiteListToWebKit(extension);
            }
        }

#if defined(UPDATE_TZ_FROM_FILE)
        _tzSupport.Initialize();
#endif
#ifdef ENABLE_CUSTOM_PROCESS_INFO
        std::string processName;
        Core::SystemInfo::GetEnvironment(std::string(_T("PROCESS_NAME")), processName);
        if (processName.empty() != true) {
            Core::ProcessInfo().Name(processName);
        }
#endif
    }

    void Deinitialize()
    {
#if defined(UPDATE_TZ_FROM_FILE)
        _tzSupport.Deinitialize();
#endif

        Messaging::MessageUnit::Instance().Close();

        if (_comClient.IsValid() == true) {
            _comClient->Close(RPC::CommunicationTimeOut);
            _comClient.Release();
        }
        if (_engine.IsValid() == true) {
            _engine.Release();
        }

        g_object_unref(_extension);
        Core::Singleton::Dispose();
    }

private:
    static void windowObjectClearedCallback(WebKitScriptWorld* world, WebKitWebPage* page VARIABLE_IS_NOT_USED, WebKitFrame* frame, VARIABLE_IS_NOT_USED PluginHost* host)
    {
        JavaScript::Milestone::InjectJS(world, frame);
        JavaScript::NotifyThunder::InjectJS(world, frame);

#ifdef  ENABLE_SECURITY_AGENT
        JavaScript::SecurityAgent::InjectJS(world, frame);
#endif

#ifdef  ENABLE_IIDENTIFIER
        JavaScript::IIdentifier::InjectJS(world, frame, host->_comClient);
#endif

#ifdef  ENABLE_BADGER_BRIDGE
        JavaScript::BridgeObject::InjectJS(world, page, frame);
#endif

#ifdef  ENABLE_AAMP_JSBINDINGS
        JavaScript::AAMP::LoadJSBindings(world, frame);
#endif

#ifdef  ENABLE_TESTING
        if (host->_enableTesting)
            JavaScript::TestRunner::InjectJS(world, page, frame);
#endif  // ENABLE_TESTING

    }
    static void pageCreatedCallback(VARIABLE_IS_NOT_USED WebKitWebExtension* webExtension,
                                    WebKitWebPage* page,
                                    PluginHost* host)
    {
        ASSERT(host != nullptr);
        g_signal_connect(page, "console-message-sent",
                G_CALLBACK(consoleMessageSentCallback), host);
        g_signal_connect(page, "user-message-received",
                G_CALLBACK(userMessageReceivedCallback), nullptr);
        g_signal_connect(page, "send-request",
                G_CALLBACK(sendRequestCallback), nullptr);

#ifdef  ENABLE_AAMP_JSBINDINGS
        g_signal_connect(page, "did-start-provisional-load-for-frame",
                G_CALLBACK(didStartProvisionalLoadForFrame), nullptr);
#endif
    }

PUSH_WARNING(DISABLE_WARNING_DEPRECATED_USE)
    static void consoleMessageSentCallback(VARIABLE_IS_NOT_USED WebKitWebPage* page, WebKitConsoleMessage* message, PluginHost* host)
    {
        ASSERT(host != nullptr);
        string messageString = Core::ToString(webkit_console_message_get_text(message));
        uint64_t line = static_cast<uint64_t>(webkit_console_message_get_line(message));

        TRACE_GLOBAL(BrowserConsoleLog, (host->_consoleLogPrefix, messageString, line, 0));
    }
POP_WARNING()

    static gboolean userMessageReceivedCallback(WebKitWebPage* page, WebKitUserMessage* message)
    {
        const char* name = webkit_user_message_get_name(message);
        if (g_strcmp0(name, Tags::Headers) == 0) {
            WebKit::SetRequestHeaders(page, message);
        }
#if defined(ENABLE_BADGER_BRIDGE)
        else if ((g_strcmp0(name, Tags::BridgeObjectReply) == 0)
                || (g_strcmp0(name, Tags::BridgeObjectEvent) == 0)) {
            JavaScript::BridgeObject::HandleMessageToPage(page, name, message);
        }
#endif
#ifdef ENABLE_TESTING
        else if (g_str_has_prefix(name, Testing::Tags::TestRunnerPrefix)) {
            JavaScript::TestRunner::HandleMessageToPage(page, message);
        }
#endif
        return TRUE;
    }
    static gboolean sendRequestCallback(WebKitWebPage* page, WebKitURIRequest* request, WebKitURIResponse*)
    {
        WebKit::ApplyRequestHeaders(page, request);
        return FALSE;
    }

#ifdef  ENABLE_AAMP_JSBINDINGS
    static gboolean didStartProvisionalLoadForFrame(WebKitWebPage* page, WebKitFrame* frame)
    {
      if (webkit_frame_is_main_frame(frame)) {
          JavaScript::AAMP::UnloadJSBindings(webkit_script_world_get_default(), frame);
      }
      return FALSE;
    }
#endif

private:
    Core::ProxyType<RPC::InvokeServerType<2, 0, 4> > _engine;
    Core::ProxyType<RPC::CommunicatorClient> _comClient;

#if defined(UPDATE_TZ_FROM_FILE)
    TZ::TimeZoneSupport _tzSupport;
#endif

    string _consoleLogPrefix;
    gboolean _enableTesting;
    WebKitWebExtension* _extension;
} _thunderClient;

extern "C" {

__attribute__((destructor)) static void unload()
{
    _thunderClient.Deinitialize();
}

// Declare module name for tracer.
MODULE_NAME_DECLARATION(BUILD_REFERENCE)

G_MODULE_EXPORT void webkit_web_extension_initialize_with_user_data(WebKitWebExtension* extension, GVariant* userData)
{
    _thunderClient.Initialize(extension, userData);
}

}

// explicit instantiation so that -O1/2/3 flags do not introduce undefined symbols
template uint32_t Thunder::Core::IPCMessageType<2u, Thunder::RPC::Data::Input, Thunder::RPC::Data::Output>::RawSerializedType<Thunder::RPC::Data::Input, 4u>::AddRef() const;
template uint32_t Thunder::Core::IPCMessageType<2u, Thunder::RPC::Data::Input, Thunder::RPC::Data::Output>::RawSerializedType<Thunder::RPC::Data::Output, 5u>::AddRef() const;
