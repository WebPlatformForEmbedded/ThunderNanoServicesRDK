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

#pragma once

/**
 * @brief C interface for a telemetry backend.
 *
 *        The backend is selected at build time (static linking) and must
 *        implement all three functions below. MessageControl calls them
 *        directly.
 *
 *        Lifecycle:
 *          1. TelemetryBackend_Configure()   — once, at plugin Initialize
 *          2. TelemetryBackend_Send()        — for every telemetry event
 *          3. TelemetryBackend_Teardown()    — once, at plugin Deinitialize
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TELEMETRY_VALUE_TEXT    = 0,
    TELEMETRY_VALUE_INT8    = 1,
    TELEMETRY_VALUE_UINT8   = 2,
    TELEMETRY_VALUE_INT16   = 3,
    TELEMETRY_VALUE_UINT16  = 4,
    TELEMETRY_VALUE_INT32   = 5,
    TELEMETRY_VALUE_UINT32  = 6,
    TELEMETRY_VALUE_INT64   = 7,
    TELEMETRY_VALUE_UINT64  = 8,
    TELEMETRY_VALUE_FLOAT32 = 9,
    TELEMETRY_VALUE_FLOAT64 = 10
} TelemetryBackend_ValueType;

/**
 * @brief One-time initialization with backend-specific settings.
 * @param configuration Null-terminated JSON string (opaque to the caller)
 * @return 0 on success, non-zero on failure
 */
uint32_t TelemetryBackend_Configure(const char* configuration);

/**
 * @brief Send a telemetry event.
 * @param category   Telemetry category name (marker)
 * @param module     Source module name
 * @param timestamp  Message timestamp (ticks)
 * @param type       Value type tag
 * @param value      For TEXT: a const char* (null-terminated string).
 *                   For signed integers: pointer to int64_t.
 *                   For unsigned integers: pointer to uint64_t.
 *                   For FLOAT32: pointer to float.
 *                   For FLOAT64: pointer to double.
 * @return 0 on success
 */
uint32_t TelemetryBackend_Send(const char* category, const char* module,
                               uint64_t timestamp,
                               TelemetryBackend_ValueType type,
                               const void* value);

/**
 * @brief Graceful shutdown — flush pending data, release resources.
 * @return 0 on success
 */
uint32_t TelemetryBackend_Teardown(void);

#ifdef __cplusplus
}
#endif
