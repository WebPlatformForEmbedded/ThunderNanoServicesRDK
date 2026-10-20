/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 Metrological
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

#include <messaging/ExternalOutput.h>

#include "rdk_logger.h"

#include <cstring>
#include <map>
#include <string>
#include <utility>

namespace {
    using ControlKey = std::pair<std::string, rdk_LogLevel>;
    using ControlMap = std::map<ControlKey, bool>;

    rdk_LogLevel RDKLevel(const ThunderMessageType type, const char* category)
    {
        switch (type) {
        case THUNDER_EXTERNAL_MESSAGE_TRACING:
            if (strcmp(category, "Notice") == 0) {
                return (RDK_LOG_NOTICE);
            } else if ((strcmp(category, "Warning") == 0) || (strcmp(category, "Warn") == 0)) {
                return (RDK_LOG_WARN);
            } else if (strcmp(category, "Error") == 0) {
                return (RDK_LOG_ERROR);
            } else if ((strcmp(category, "Fatal") == 0) || (strcmp(category, "Crash") == 0)) {
                return (RDK_LOG_FATAL);
            } else {
                return (RDK_LOG_TRACE);
            }
        case THUNDER_EXTERNAL_MESSAGE_LOGGING:
            if (strcmp(category, "Notice") == 0) {
                return (RDK_LOG_NOTICE);
            } else if ((strcmp(category, "Warning") == 0) || (strcmp(category, "Warn") == 0)) {
                return (RDK_LOG_WARN);
            } else if (strcmp(category, "Error") == 0) {
                return (RDK_LOG_ERROR);
            } else if ((strcmp(category, "Fatal") == 0) || (strcmp(category, "Crash") == 0)) {
                return (RDK_LOG_FATAL);
            } else {
                return (RDK_LOG_INFO);
            }
        case THUNDER_EXTERNAL_MESSAGE_OPERATIONAL_STREAM:
            return (RDK_LOG_TRACE);
        case THUNDER_EXTERNAL_MESSAGE_TELEMETRY:
            return (RDK_LOG_NOTICE);
        case THUNDER_EXTERNAL_MESSAGE_REPORTING:
            return (RDK_LOG_WARN);
        case THUNDER_EXTERNAL_MESSAGE_ASSERT:
            return (RDK_LOG_FATAL);
        default:
            return (RDK_LOG_INFO);
        }
    }

    std::string Module(const char* module)
    {
        return (std::string("LOG.RDK.THUNDER.") + module);
    }

    ControlMap& Controls()
    {
        static ControlMap controls;
        return (controls);
    }
}

extern "C" {
    bool ThunderExternalOutput_Initialize(void)
    {
        Controls().clear();
        const rdk_Error result = RDK_LOGGER_INIT();
        return (result == RDK_SUCCESS);
    }

    bool ThunderExternalOutput_IsEnabled(const char* module, const ThunderMessageType type, const char* category)
    {
        const std::string moduleName = Module(module);
        const rdk_LogLevel level = RDKLevel(type, category);
        const ControlKey key { moduleName, level };
        const auto entry = Controls().find(key);
        bool enabled = false;

        if (entry != Controls().end()) {
            enabled = entry->second;
        }
        else {
            enabled = (rdk_logger_is_logLevel_enabled(moduleName.c_str(), level) == TRUE);
            Controls().emplace(key, enabled);
        }

        return (enabled);
    }

    void ThunderExternalOutput_Submit(const char* module, const ThunderMessageType type, const char* category, const char* payload)
    {
        const std::string moduleName = Module(module);
        const rdk_LogLevel level = RDKLevel(type, category);

        if (((type == THUNDER_EXTERNAL_MESSAGE_LOGGING) && (level == RDK_LOG_INFO)) ||
            ((type == THUNDER_EXTERNAL_MESSAGE_TRACING) && (level == RDK_LOG_TRACE))) {
            RDK_LOG(level, moduleName.c_str(), "%s: %s", category, payload);
        }
        else {
            RDK_LOG(level, moduleName.c_str(), "%s", payload);
        }
    }

    void ThunderExternalOutput_Deinitialize(void)
    {
        Controls().clear();
        rdk_logger_deinit();
    }
}
