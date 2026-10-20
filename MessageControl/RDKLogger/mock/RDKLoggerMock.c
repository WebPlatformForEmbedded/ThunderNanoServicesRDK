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

#include <rdk_logger.h>

#include <stdarg.h>
#include <stdio.h>

rdk_Error rdk_logger_init(const char* configuration)
{
    printf("[rdk-logger-mock] init(%s)\n", configuration ? configuration : "");
    return (RDK_SUCCESS);
}

rdk_Error rdk_logger_deinit(void)
{
    printf("[rdk-logger-mock] deinit()\n");
    return (RDK_SUCCESS);
}

bool rdk_logger_is_logLevel_enabled(const char* module, const rdk_LogLevel level)
{
    (void)module;
    (void)level;
    return (true);
}

void rdk_logger_msg_printf(const rdk_LogLevel level, const char* module, const char* format, ...)
{
    va_list arguments;
    printf("[rdk-logger-mock] [module=%s] [level=%d] ", module, level);
    va_start(arguments, format);
    vprintf(format, arguments);
    va_end(arguments);
}