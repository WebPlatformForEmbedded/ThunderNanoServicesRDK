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
        Register<MessageData, void>(_T("set"), &MessageControl::endpoint_set, this);
    }

    void MessageControl::UnregisterAll()
    {
        Unregister(_T("set"));
    }

    // API implementation
    //

    // Method: set - Sets messages
    // Return codes:
    //  - ERROR_NONE: Success
    uint32_t MessageControl::endpoint_set(const MessageData& params)
    {
        uint32_t result = Core::ERROR_NONE;
        if (_control != nullptr) {
            auto state = params.State.Value() == MessageData::StateType::ENABLED ? true : false;
            result = _control->EnableMessage(static_cast<Exchange::IMessageControl::MessageType>(params.Type.Value()), params.Module.Value(), params.Category.Value(), state);
        }
        return result;
    }
} // namespace Plugin

}
