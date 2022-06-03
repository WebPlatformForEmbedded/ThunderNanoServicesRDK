/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 RDK Management
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

#include "MessageControl.h"
#include "MessageOutput.h"

namespace WPEFramework {

    namespace Plugin {

    SERVICE_REGISTRATION(MessageControl, 1, 0);
    
    MessageControl::Config::NetworkNode::NetworkNode()
        : Core::JSON::Container()
        , Port(2200)
        , Binding("0.0.0.0")
    {
        Add(_T("port"), &Port);
        Add(_T("binding"), &Binding);
    }

    MessageControl::Config::NetworkNode::NetworkNode(const NetworkNode& copy)
        : Core::JSON::Container()
        , Port(copy.Port)
        , Binding(copy.Binding)
    {
        Add(_T("port"), &Port);
        Add(_T("binding"), &Binding);
    }

    MessageControl::MessageControl()
        : _dispatcher()
    {
    }

    const string MessageControl::Initialize(PluginHost::IShell* service)
    {
        ASSERT(service != nullptr);

        _config.Clear();
        _config.FromString(service->ConfigLine());

        RegisterAll();

        string message = _dispatcher.Initialize(service, _config);

        if(message.length() != 0) {
            Deinitialize(service);
        }

        return message;
    }

    void MessageControl::Deinitialize(PluginHost::IShell* service)
    {
        UnregisterAll();

        _dispatcher.Deinitialize();
    }

    string MessageControl::Information() const
    {
        // No additional info to report.
        return (string());
    }

    bool MessageControl::Attach(PluginHost::Channel& channel)
    {
        TRACE(Trace::Information, (Core::Format(_T("Attaching channel ID [%d]"), channel.Id()).c_str()));
        return (_dispatcher.Subscribe(channel.Id()));
    }

    void MessageControl::Detach(PluginHost::Channel& channel)
    {
        TRACE(Trace::Information, (Core::Format(_T("Detaching channel ID [%d]"), channel.Id()).c_str()));
        _dispatcher.Unsubscribe(channel.Id());
    }

    Core::ProxyType<Core::JSON::IElement> MessageControl::Inbound(const string&)
    {
        return (_dispatcher.Command());
    }

    Core::ProxyType<Core::JSON::IElement> MessageControl::Inbound(const uint32_t ID, const Core::ProxyType<Core::JSON::IElement>& element)
    {
        return (Core::ProxyType<Core::JSON::IElement>(_dispatcher.Received(ID, element)));
    }

} // namespace Plugin
} // namespace WPEFramework
