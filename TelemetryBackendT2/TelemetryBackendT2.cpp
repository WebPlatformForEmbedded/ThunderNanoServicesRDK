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

// Forward declarations for the t2 API
#if !defined(T2_STUB)
extern "C" {
    void t2_init(char* component);
    int t2_event_s(const char* marker, const char* value);
    int t2_event_d(const char* marker, int value);
    int t2_event_f(const char* marker, double value);
    void t2_uninit(void);
}
#else
// Stub implementations when the t2 library is not available
static inline void t2_init(char*) {}
static inline int t2_event_s(const char*, const char*) { return 0; }
static inline int t2_event_d(const char*, int) { return 0; }
static inline int t2_event_f(const char*, double) { return 0; }
static inline void t2_uninit() {}
#endif

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

uint32_t TelemetryBackend_SendString(const char* category, const char* /* module */, uint64_t /* timestamp */, const char* message)
{
    uint32_t result = 1;

    if (g_initialized == true) {
        result = (t2_event_s(category, message) == 0) ? 0 : 1;
    }

    return (result);
}

uint32_t TelemetryBackend_SendInteger(const char* category, const char* /* module */, uint64_t /* timestamp */, int64_t value)
{
    uint32_t result = 1;

    if (g_initialized == true) {
        if ((value >= INT_MIN) && (value <= INT_MAX)) {
            result = (t2_event_d(category, static_cast<int>(value)) == 0) ? 0 : 1;
        }
    }

    return (result);
}

uint32_t TelemetryBackend_SendFloat(const char* category, const char* /* module */, uint64_t /* timestamp */, double value)
{
    uint32_t result = 1;

    if (g_initialized == true) {
        result = (t2_event_f(category, value) == 0) ? 0 : 1;
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
