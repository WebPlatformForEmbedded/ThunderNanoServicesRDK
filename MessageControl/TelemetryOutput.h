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
 *        implement all five functions below. MessageControl calls them
 *        directly.
 *
 *        Lifecycle:
 *          1. TelemetryBackend_Configure()   — once, at plugin Initialize
 *          2. TelemetryBackend_Send*()       — for every telemetry event
 *          3. TelemetryBackend_Teardown()    — once, at plugin Deinitialize
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief One-time initialization with backend-specific settings.
 * @param configuration Null-terminated JSON string (opaque to the caller)
 * @return 0 on success, non-zero on failure
 */
uint32_t TelemetryBackend_Configure(const char* configuration);

/**
 * @brief Send a string telemetry event.
 * @param category   Telemetry category name (marker)
 * @param module     Source module name
 * @param timestamp  Message timestamp (ticks)
 * @param message    Null-terminated message text
 * @return 0 on success
 */
uint32_t TelemetryBackend_SendString(const char* category, const char* module, uint64_t timestamp, const char* message);

/**
 * @brief Send an integer telemetry event.
 */
uint32_t TelemetryBackend_SendInteger(const char* category, const char* module, uint64_t timestamp, int64_t value);

/**
 * @brief Send a floating-point telemetry event.
 */
uint32_t TelemetryBackend_SendFloat(const char* category, const char* module, uint64_t timestamp, double value);

/**
 * @brief Graceful shutdown — flush pending data, release resources.
 * @return 0 on success
 */
uint32_t TelemetryBackend_Teardown(void);

#ifdef __cplusplus
}
#endif
