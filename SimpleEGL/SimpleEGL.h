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
 
#ifndef __SIMPLEEGL_H
#define __WIMPLEEGL_H

#include "Module.h"
#include <interfaces/IBrowser.h>

namespace WPEFramework {
namespace Plugin {

    class SimpleEGL : public PluginHost::IPluginExtended, public PluginHost::IWeb {
    private:
        SimpleEGL(const SimpleEGL&);
        SimpleEGL& operator=(const SimpleEGL&);

        class Notification : public Exchange::IBrowser::INotification,
                             public PluginHost::IStateControl::INotification,
                             public RPC::IRemoteConnection::INotification {
        private:
            Notification();
            Notification(const Notification&);
            Notification& operator=(const Notification&);

        public:
            explicit Notification(SimpleEGL* parent)
                : _parent(*parent)
            {
                ASSERT(parent != nullptr);
                fprintf(stderr, "================ Constructing the Notification =================\n"); fflush(stderr);

            }
            ~Notification()
            {
                fprintf(stderr, "================ Destructing the Notification =================\n"); fflush(stderr);
                TRACE_L1("WebServer::Notification destructed. Line: %d", __LINE__);
            }

        public:
            virtual void LoadFinished(const string& URL)
            {
                _parent.LoadFinished(URL);
            }
            virtual void URLChanged(const string& URL)
            {
                _parent.URLChanged(URL);
            }
            virtual void Hidden(const bool hidden)
            {
                _parent.Hidden(hidden);
            }
            virtual void StateChange(const PluginHost::IStateControl::state value)
            {
                _parent.StateChange(value);
            }
            virtual void Activated(RPC::IRemoteConnection* /* connection */)
            {
            }
            virtual void Deactivated(RPC::IRemoteConnection* connection)
            {
                _parent.Deactivated(connection);
            }
            virtual void Closure()
            {
            }

            BEGIN_INTERFACE_MAP(Notification)
            INTERFACE_ENTRY(Exchange::IBrowser::INotification)
            INTERFACE_ENTRY(PluginHost::IStateControl::INotification)
            INTERFACE_ENTRY(RPC::IRemoteConnection::INotification)
            END_INTERFACE_MAP

        private:
            SimpleEGL& _parent;
        };

        class Config : public Core::JSON::Container {
        private:
            Config(const Config&);
            Config& operator=(const Config&);

        public:
            Config()
                : Core::JSON::Container()
                , SimpleEGL(true)
            {
                Add(_T("outofprocess"), &SimpleEGL);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::Boolean SimpleEGL;
        };

    public:
        class Data : public Core::JSON::Container {
        private:
            Data(const Data&);
            Data& operator=(const Data&);

        public:
            Data()
                : Core::JSON::Container()
                , URL()
                , FPS()
                , Suspended(false)
                , Hidden(false)
            {
                Add(_T("url"), &URL);
                Add(_T("fps"), &FPS);
                Add(_T("suspended"), &Suspended);
                Add(_T("hidden"), &Hidden);
            }

            Data(const string& url)
                : Core::JSON::Container()
                , URL()
                , FPS()
                , Suspended(false)
                , Hidden(false)
            {
                Add(_T("url"), &URL);
                Add(_T("fps"), &FPS);
                Add(_T("suspended"), &Suspended);
                Add(_T("hidden"), &Hidden);
                URL = url;
            }
            ~Data()
            {
            }

        public:
            Core::JSON::String URL;
            Core::JSON::DecUInt32 FPS;
            Core::JSON::Boolean Suspended;
            Core::JSON::Boolean Hidden;
        };

    public:
#ifdef __WINDOWS__
#pragma warning(disable : 4355)
#endif
        SimpleEGL()
            : _adminLock()
            , _skipURL(0)
            , _webPath("")
            , _service(nullptr)
            , _browser(nullptr)
            , _notification(Core::Service<Notification>::Create<Notification>(this))
            , _state(PluginHost::IStateControl::UNINITIALIZED)
            , _subscriber(nullptr)
            , _hidden(false)
        {
        }
#ifdef __WINDOWS__
#pragma warning(default : 4355)
#endif
        virtual ~SimpleEGL()
        {
            _notification->Release();
        }

    public:
        BEGIN_INTERFACE_MAP(SimpleEGL)
        INTERFACE_ENTRY(IPlugin)
        INTERFACE_ENTRY(IPluginExtended)
        INTERFACE_ENTRY(IWeb)
        INTERFACE_AGGREGATE(Exchange::IBrowser, _browser)
        INTERFACE_AGGREGATE(PluginHost::IStateControl, _browser)
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
        virtual const string Initialize(PluginHost::IShell* service);

        // The plugin is unloaded from the webbridge. This is call allows the module to notify clients
        // or to persist information if needed. After this call the plugin will unlink from the service path
        // and be deactivated. The Service object is the same as passed in during the Initialize.
        // After theis call, the lifetime of the Service object ends.
        virtual void Deinitialize(PluginHost::IShell* service);

        // Returns an interface to a JSON struct that can be used to return specific metadata information with respect
        // to this plugin. This Metadata can be used by the MetData plugin to publish this information to the ouside world.
        virtual string Information() const;

        // ================================== CALLED ON COMMUNICATION THREAD =====================================
        // Whenever a Channel (WebSocket connection) is created to the plugin that will be reported via the Attach.
        // Whenever the channel is closed, it is reported via the detach method.
        virtual bool Attach(PluginHost::Channel& channel);
        virtual void Detach(PluginHost::Channel& channel);

        //  IWeb methods
        // -------------------------------------------------------------------------------------------------------
        virtual void Inbound(Web::Request& request);
        virtual Core::ProxyType<Web::Response> Process(const Web::Request& request);

    private:
        void LoadFinished(const string& URL);
        void URLChanged(const string& URL);
        void Hidden(const bool hidden);
        void StateChange(const PluginHost::IStateControl::state value);
        void ConnectionTermination(uint32_t connection);
        void Deactivated(RPC::IRemoteConnection* connection);

    private:
        Core::CriticalSection _adminLock;
        uint8_t _skipURL;
        uint32_t _connectionId;
        string _webPath;
        PluginHost::IShell* _service;
        Exchange::IBrowser* _browser;
        Notification* _notification;
        PluginHost::IStateControl::state _state;
        PluginHost::Channel* _subscriber;
        bool _hidden;
    };
}
}

#endif // __SIMPLEEGL_H
