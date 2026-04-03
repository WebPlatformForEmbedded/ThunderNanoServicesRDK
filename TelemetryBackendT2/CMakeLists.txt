# If not stated otherwise in this file or this component's LICENSE file the
# following copyright and licenses apply:
#
# Copyright 2022 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

project(TelemetryBackendT2)

cmake_minimum_required(VERSION 3.15)

find_package(Thunder)

project_version(1.0.0)

set(MODULE_NAME ${NAMESPACE}${PROJECT_NAME})

message("Setup ${MODULE_NAME} v${PROJECT_VERSION}")

find_package(${NAMESPACE}Core REQUIRED)
find_package(CompileSettingsDebug CONFIG REQUIRED)

option(TELEMETRY_BACKEND_T2_USE_MOCK "Build and link the mock t2 library for testing" ON)

# TO-DO: Adjust the name of the T2 backend library
if(TELEMETRY_BACKEND_T2_USE_MOCK)
    add_subdirectory(mock)
    set(t2_FOUND TRUE)
else()
    find_package(t2 QUIET)
endif()

if(NOT t2_FOUND)
    message(STATUS "t2 library not found — ${MODULE_NAME} will build without linking t2 (stub mode)")
endif()

add_library(${MODULE_NAME} STATIC
    TelemetryBackendT2.cpp)

set_target_properties(${MODULE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES)

target_include_directories(${MODULE_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../MessageControl>)

target_link_libraries(${MODULE_NAME}
    PRIVATE
        CompileSettingsDebug::CompileSettingsDebug
        ${NAMESPACE}Core::${NAMESPACE}Core)

if(t2_FOUND)
    target_link_libraries(${MODULE_NAME}
        PRIVATE
            t2::t2)
else()
    target_compile_definitions(${MODULE_NAME}
        PRIVATE
            T2_STUB=1)
endif()
