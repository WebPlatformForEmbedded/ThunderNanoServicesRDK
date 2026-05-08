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

cmake_minimum_required(VERSION 3.3)

find_package(WPEFramework)

project_version(1.0.0)

set(MODULE_NAME ${NAMESPACE}${PROJECT_NAME})

message("Setup ${MODULE_NAME} v${PROJECT_VERSION}")

find_package(${NAMESPACE}Core REQUIRED)
find_package(CompileSettingsDebug CONFIG REQUIRED)

option(TELEMETRY_BACKEND_T2_USE_MOCK "Build and link the mock telemetry_msgsender library for testing" OFF)

if(TELEMETRY_BACKEND_T2_USE_MOCK)
    add_subdirectory(mock)
    set(telemetry_msgsender_FOUND TRUE)
else()
    find_package(telemetry_msgsender QUIET)
endif()

if(NOT telemetry_msgsender_FOUND)
    message(FATAL_ERROR "telemetry_msgsender library not found — set TELEMETRY_BACKEND_T2_USE_MOCK=ON to build with the mock library")
endif()

add_library(${MODULE_NAME} STATIC
    TelemetryBackendT2.cpp)

set_target_properties(${MODULE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    POSITION_INDEPENDENT_CODE ON)

target_include_directories(${MODULE_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)

target_link_libraries(${MODULE_NAME}
    PRIVATE
        CompileSettingsDebug::CompileSettingsDebug
        ${NAMESPACE}Core::${NAMESPACE}Core
        telemetry_msgsender::telemetry_msgsender)
