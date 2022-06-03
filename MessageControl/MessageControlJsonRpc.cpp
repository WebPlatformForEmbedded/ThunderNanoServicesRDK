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
#include "Module.h"
#include <interfaces/json/JsonData_MessageControl.h>

namespace WPEFramework {

namespace Plugin {

    using namespace JsonData::MessageControl;

    // Registration
    //

    void MessageControl::RegisterAll()
    {
        Register<MessageInfo, void>(_T("set"), &MessageControl::endpoint_set, this);
        Register<StatusParamsData, StatusResultData>(_T("status"), &MessageControl::endpoint_status, this);
    }

    void MessageControl::UnregisterAll()
    {
        Unregister(_T("set"));
        Unregister(_T("status"));
    }

    // API implementation
    //

    // Method: set - Sets messages
    // Return codes:
    //  - ERROR_NONE: Success
    uint32_t MessageControl::endpoint_set(const MessageInfo& params)
    {
        uint32_t result = Core::ERROR_NONE;
        auto state = params.State.Value() == StateType::ENABLED ? true : false;
        return(_control->Enable(static_cast<Exchange::IMessageControl::MessageType>(params.Type.Value()), params.Module.Value(), params.Category.Value(), state));
    }

    // Method: status - Retrieves general information
    // Return codes:
    //  - ERROR_NONE: Success
    uint32_t MessageControl::endpoint_status(const StatusParamsData& params, StatusResultData& response)
    {
        uint32_t result = Core::ERROR_NONE;

        Exchange::IMessageControl::MessageType type;
        string module;
        string category;
        bool enabled;
        bool add = false;
        bool initialize = true;

        if (!params.IsSet()) {
            response.Console = _config.Console;
            response.Syslog = _config.SysLog;
            response.FileNameOutput = _config.FileName;
            response.Abbreviated = _config.Abbreviated;
            response.Maxexportconnections = _webSocketExporter.MaxConnections();;
            if (_config.Remote.IsSet()) {
                response.Remote.Binding = _config.Remote.Binding;
                response.Remote.Port = _config.Remote.Port;
            }
        }

        while (_control->Setting(initialize, type, module, category, enabled) == Core::ERROR_NONE) {
            if (!params.IsSet()) {
                add = true;
            } else {
                if (params.Type.IsSet() && params.Type.Value() == static_cast<Message_typeType>(type)) {
                    if (params.Module.IsSet() && !params.Category.IsSet()) {
                        if (params.Module.Value() == module) {
                            add = true;
                        }
                    } else if (!params.Module.IsSet() && params.Category.IsSet()) {
                        if (params.Category.Value() == category) {
                            add = true;
                        }
                    } else if (params.Module.IsSet() && params.Category.IsSet()) {
                        if (params.Category.Value() == category && params.Module.Value() == module) {
                            add = true;
                        }
                    } else if (!params.Module.IsSet() && !params.Category.IsSet()) {
                        add = true;
                    }
                }
            }

            if (add) {
                MessageInfo messageResponse;
                messageResponse.Type = static_cast<Message_typeType>(type);
                messageResponse.Module = module;
                messageResponse.Category = category;
                messageResponse.State = enabled ? StateType::ENABLED : StateType::DISABLED;
                response.Messages.Add(messageResponse);
                add = false;
            }
            initialize = false;
        }

        return result;
    }
} // namespace Plugin

}
