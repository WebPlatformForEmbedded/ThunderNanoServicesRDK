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

namespace WPEFramework {
namespace Plugin {

    class PluginActivator : public PluginHost::IPlugin {

    private:
        class Config : public Core::JSON::Container {
        public:
            Config(const Config&);
            Config& operator=(const Config&);

        public:
            Config()
                : Core::JSON::Container()
                , ObservableCallsign()
            {
                Add(_T("callsign"), &ObservableCallsign);
                Add(_T("retries"), &Retries);
            }

        public:
            Core::JSON::String ObservableCallsign;
            Core::JSON::DecUInt8 Retries;
        };

            class ActivatePluginJob {
            public:
            explicit ActivatePluginJob()
                : _plugin(nullptr)
                , _pooljob(*this)
            {
            }
            ~ActivatePluginJob() {
                Revoke();
            }

            ActivatePluginJob(const ActivatePluginJob&) = delete;
            ActivatePluginJob& operator=(const ActivatePluginJob&) = delete;

            void Activate(PluginHost::IShell* plugin)
            {
                ASSERT(plugin != nullptr && _plugin == nullptr);
                _plugin = plugin;
                _pooljob.Submit();
            }

            void Activate(const uint32_t milliseconds)
            {
                _pooljob.Reschedule(Core::Time::Now().Add(milliseconds));
            }

            PluginHost::IShell* PluginShell() const 
            {
                return _plugin;
            }

            void Revoke()
            {
                _pooljob.Revoke();
                if( _plugin != nullptr) {
                    _plugin->Release();
                    _plugin = nullptr;
                }
            }

            private:
            void Dispatch()
            {
                ASSERT(_plugin != nullptr);

                if( _plugin->State() != PluginHost::IShell::ACTIVATED ) {
                    uint32_t res = _plugin->Activate(PluginHost::IShell::REQUESTED);
                    TRACE(Trace::Information, (_T("plugin [%s] activated, result: %u"), _plugin->Callsign().c_str(), res));
                }
                else {
                    TRACE(Trace::Information, (_T("plugin [%s] was already activated"), _plugin->Callsign().c_str()));
                }
            }

            private:
            PluginHost::IShell* _plugin;
            friend Core::ThreadPool::JobType<ActivatePluginJob&>;
            Core::WorkerPool::JobType<ActivatePluginJob&> _pooljob;
        };

        class Notification : public PluginHost::IPlugin::INotification {
        public:
            Notification(const Notification&) = delete;
            Notification& operator=(const Notification&) = delete;

            explicit Notification(PluginActivator& parent)
                : PluginHost::IPlugin::INotification()
                , _parent(parent)
            {
            }
            ~Notification() override = default;

            void Activated (const string&, PluginHost::IShell* service) override
            {
            }
            void Deactivated (const string&, PluginHost::IShell* service) override
            {
            }
            void Unavailable(const string&, PluginHost::IShell*) override
            {
            }
            void Deinitialized(const string& callsign, PluginHost::IShell* plugin) override 
            {
                _parent.PluginDeinitialized(callsign, plugin);
            }


            BEGIN_INTERFACE_MAP(Notification)
            INTERFACE_ENTRY(PluginHost::IPlugin::INotification)
            END_INTERFACE_MAP

        private:
            PluginActivator& _parent;
        };

    public:

PUSH_WARNING(DISABLE_WARNING_THIS_IN_MEMBER_INITIALIZER_LIST)
        PluginActivator()
        : PluginHost::IPlugin()
        , _notification(*this)
        , _callsign()
        , _retries(0)
        , _activatejob()
        {
        }
POP_WARNING()
        ~PluginActivator() override = default;

        PluginActivator(const PluginActivator&) = delete;
        PluginActivator& operator=(const PluginActivator&) = delete;

        BEGIN_INTERFACE_MAP(PluginActivator)
        INTERFACE_ENTRY(PluginHost::IPlugin)
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

        // The plugin is unloaded from WPEFramework. This is call allows the module to notify clients
        // or to persist information if needed. After this call the plugin will unlink from the service path
        // and be deactivated. The Service object is the same as passed in during the Initialize.
        // After theis call, the lifetime of the Service object ends.
        virtual void Deinitialize(PluginHost::IShell* service);

        // Returns an interface to a JSON struct that can be used to return specific metadata information with respect
        // to this plugin. This Metadata can be used by the MetData plugin to publish this information to the ouside world.
        virtual string Information() const;

    private:
        void PluginDeinitialized(const string& callsign, PluginHost::IShell* plugin);

    private:
        Core::Sink<Notification> _notification;
        string _callsign;
        uint8_t _retries;
        ActivatePluginJob _activatejob;
    };

}
}



