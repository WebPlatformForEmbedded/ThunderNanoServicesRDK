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
#include "Linchpin.h"
#include "cryptalgo/Hash.h"

namespace WPEFramework {

namespace Plugin {

    namespace {

        static Metadata<Linchpin> metadata(
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

    // IPlugin methods

    const string Linchpin::Initialize(PluginHost::IShell* service VARIABLE_IS_NOT_USED)
    {
        string message;

        RegisterEventStatusListener(_T("message"), [this](const string& clientid, Status status) {
            string topicId = clientid.substr(0, clientid.find('.'));
            if(status == Status::registered) {
                OnMessageSubscribed(topicId);
            } else {
                OnMessageUnsubscribed(topicId);
            }
        });

        Exchange::JTopicExchange::Register(*this, this);

        return message;
    }

    void Linchpin::Deinitialize(PluginHost::IShell* service VARIABLE_IS_NOT_USED)
    {
        Exchange::JTopicExchange::Unregister(*this);
        UnregisterEventStatusListener(_T("message"));
    }

    // Web request handlers

} // namespace Plugin

} // WPEFramework

