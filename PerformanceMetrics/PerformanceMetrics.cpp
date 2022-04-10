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
 
#include "PerformanceMetrics.h"

namespace WPEFramework {
namespace Plugin {

    SERVICE_REGISTRATION(PerformanceMetrics, 1, 0);

    const string PerformanceMetrics::Initialize(PluginHost::IShell* service)
    {
        ASSERT(service != nullptr);

        Config config;
        config.FromString(service->ConfigLine());

        string result;

        if( ( config.ObservableCallsign.IsSet() == true ) && ( config.ObservableCallsign.Value().empty() == false ) ) {
            _callsign = config.ObservableCallsign.Value();
            service->Register(&_notification);
        } else {
            result = _T("No callsign set to observe for metrics");
        }

        if( result.empty() == false ) {
            Deinitialize(service);
        }

        return result;
    }

    void PerformanceMetrics::Deinitialize(PluginHost::IShell* service)
    {
        if( _callsign.empty() == false ) {
            service->Unregister(&_notification);
            // as we do this after the unregister this should be threadsafe
            // if the deactivate of the observable did not happen we must clean up here
            if( _observable.IsValid() == true ) {
                _observable->Disable();
                VARIABLE_IS_NOT_USED uint32_t result =_observable.Release(); 
                ASSERT(result == Core::ERROR_DESTRUCTION_SUCCEEDED);
                ASSERT(_observable.IsValid() == false);
            }
            _callsign.clear();
        }
    }

    string PerformanceMetrics::Information() const
    {
        return (_T(""));
    }

    void PerformanceMetrics::ObservableActivated(PluginHost::IShell& service) 
    {
        ASSERT(_observable.IsValid() == false);

        CreateObservable(service);
        _observable->Activated(service);
    }

    void PerformanceMetrics::ObservableDeactivated(PluginHost::IShell& service) 
    {
        ASSERT(_observable.IsValid() == true);

        _observable->Deactivated(service);
        _observable->Disable();
        VARIABLE_IS_NOT_USED uint32_t result =_observable.Release(); 
        ASSERT(result == Core::ERROR_DESTRUCTION_SUCCEEDED);
        ASSERT(_observable.IsValid() == false);
    }

    constexpr char PerformanceMetrics::IBrowserMetricsLogger::startURL[];

}
}
