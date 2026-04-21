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

#include "../MessageControl/TelemetryOutput.h"

#ifndef MODULE_NAME
#define MODULE_NAME TelemetryBackendT2
#endif

#include <core/core.h>
#include <climits>
#include <string>
#include <vector>

#include <telemetry_busmessage_sender.h>

static bool g_initialized = false;

/**
 * @brief Telemetry backend implemented against the t2 API.
 *
 *        Configuration JSON (passed as null-terminated string):
 *        {
 *            "component": "Thunder"    // component name for t2_init
 *        }
 */

extern "C" {

uint32_t TelemetryBackend_Configure(const char* configuration)
{
    std::string component("Thunder");

    if (configuration != nullptr && configuration[0] != '\0') {
        Thunder::Core::JSON::String jsonComponent;
        Thunder::Core::JSON::Container config;
        config.Add(_T("component"), &jsonComponent);
        config.FromString(std::string(configuration));

        if (jsonComponent.IsSet() == true) {
            component = jsonComponent.Value();
        }
    }

    std::vector<char> componentBuf(component.begin(), component.end());
    componentBuf.push_back('\0');
    t2_init(componentBuf.data());

    g_initialized = true;

    return (0);
}

uint32_t TelemetryBackend_Send(const char* category, const char* /* module */,
                               uint64_t /* timestamp */,
                               TelemetryBackend_ValueType type,
                               const void* value)
{
    uint32_t result = 1;

    if (g_initialized == true) {
        switch (type) {
        case TELEMETRY_VALUE_TEXT:
            result = (t2_event_s(category, static_cast<const char*>(value)) == 0) ? 0 : 1;
            break;
        case TELEMETRY_VALUE_INT8:
        case TELEMETRY_VALUE_INT16:
        case TELEMETRY_VALUE_INT32: {
            int64_t v = *static_cast<const int64_t*>(value);
            result = (t2_event_d(category, static_cast<int>(v)) == 0) ? 0 : 1;
            break;
        }
        case TELEMETRY_VALUE_INT64: {
            int64_t v = *static_cast<const int64_t*>(value);
            if ((v >= INT_MIN) && (v <= INT_MAX)) {
                result = (t2_event_d(category, static_cast<int>(v)) == 0) ? 0 : 1;
            }
            break;
        }
        case TELEMETRY_VALUE_UINT8:
        case TELEMETRY_VALUE_UINT16: {
            uint64_t v = *static_cast<const uint64_t*>(value);
            result = (t2_event_d(category, static_cast<int>(v)) == 0) ? 0 : 1;
            break;
        }
        case TELEMETRY_VALUE_UINT32:
        case TELEMETRY_VALUE_UINT64: {
            uint64_t v = *static_cast<const uint64_t*>(value);
            if (v <= static_cast<uint64_t>(INT_MAX)) {
                result = (t2_event_d(category, static_cast<int>(v)) == 0) ? 0 : 1;
            }
            break;
        }
        case TELEMETRY_VALUE_FLOAT32: {
            float v = *static_cast<const float*>(value);
            result = (t2_event_f(category, static_cast<double>(v)) == 0) ? 0 : 1;
            break;
        }
        case TELEMETRY_VALUE_FLOAT64: {
            double v = *static_cast<const double*>(value);
            result = (t2_event_f(category, v) == 0) ? 0 : 1;
            break;
        }
        default:
            break;
        }
    }

    return (result);
}

uint32_t TelemetryBackend_Teardown(void)
{
    if (g_initialized == true) {
        t2_uninit();
        g_initialized = false;
    }

    return (0);
}

} // extern "C"
