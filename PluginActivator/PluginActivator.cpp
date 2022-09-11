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
 
#include "PluginActivator.h"

namespace WPEFramework {
namespace Plugin {

    namespace {

        static Metadata<PluginActivator> metadata(
            // Version
            1, 0, 0,
            // Preconditions
            {},
            // Terminations
            {},
            // Controls
            {} 
        );
    }

    const string PluginActivator::Initialize(PluginHost::IShell* service)
    {
        ASSERT(service != nullptr);

        Config config;
        config.FromString(service->ConfigLine());

        _callsign = config.ObservableCallsign.Value();
        _retries = ( config.Retries.Value() == 0 ? 1 : config.Retries.Value());

        string result;

        TRACE(Trace::Information, (_T("Activating [%s]"), _callsign.c_str()));

        PluginHost::IShell* plugin = service->QueryInterfaceByCallsign<PluginHost::IShell>(_callsign);

        if( plugin != nullptr ) {
            service->Register(&_notification);
            TRACE(Trace::Information, (_T("Shell aquired for plugin [%s]"), _callsign.c_str()));
            _activatejob.Activate(plugin);
            --_retries;
        } else {
            TRACE(Trace::Error, (_T("Could not aquire Shellfor plugin [%s]"), _callsign.c_str()));
        }

        return result;
    }

    void PluginActivator::Deinitialize(PluginHost::IShell* service)
    {
        if( _activatejob.PluginShell() != nullptr ) {
            _activatejob.Revoke();
            service->Unregister(&_notification);
        }
    }

    string PluginActivator::Information() const
    {
        return (_T(""));
    }

    void PluginActivator::PluginDeinitialized(const string& callsign, PluginHost::IShell* plugin)
    {

        if( callsign == _callsign ) {

            if( _retries != 0 ) {
                if( plugin->Reason() == PluginHost::IShell::INITIALIZATION_FAILED ) {
                    --_retries;
                    TRACE(Trace::Error, (_T("Delayed Restaring plugin [%s]"), callsign.c_str()));
                    _activatejob.Activate(3000);
                } else {
                    TRACE(Trace::Error, (_T("Not restarting plugin [%s] because of reason [%u]  "), callsign.c_str(), plugin->Reason()));
                }
            } else {
                TRACE(Trace::Error, (_T("Could not start plugin [%s] within specified number of retries"), callsign.c_str()));
            }
        }
    }

}
}
