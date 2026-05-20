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

cmake_minimum_required(VERSION 3.3)

project(telemetry_msgsender_mock C)

# Download the real upstream headers at configure time so the mock always
# matches the actual t2 API. Both files are needed:
#   telemetry_busmessage_sender.h  — the public API we include in TelemetryBackendT2
#   telemetry2_0.h                 — defines T2ERROR, included by the above
set(T2_HEADER_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
set(T2_BASE_URL "https://raw.githubusercontent.com/rdkcentral/telemetry/develop/include")

foreach(T2_HEADER telemetry_busmessage_sender.h telemetry2_0.h)
    set(T2_DEST "${T2_HEADER_DIR}/${T2_HEADER}")
    if(NOT EXISTS "${T2_DEST}")
        message(STATUS "Downloading ${T2_HEADER} from upstream telemetry repo...")
        file(DOWNLOAD
            "${T2_BASE_URL}/${T2_HEADER}"
            "${T2_DEST}"
            STATUS T2_DOWNLOAD_STATUS)
        list(GET T2_DOWNLOAD_STATUS 0 T2_DOWNLOAD_CODE)
        if(NOT T2_DOWNLOAD_CODE EQUAL 0)
            list(GET T2_DOWNLOAD_STATUS 1 T2_DOWNLOAD_ERROR)
            message(FATAL_ERROR "Failed to download ${T2_HEADER}: ${T2_DOWNLOAD_ERROR}")
        endif()
    endif()
endforeach()

add_library(telemetry_msgsender SHARED t2_mock.c)

# Expose the downloaded headers so that #include <telemetry_busmessage_sender.h>
# resolves when building against the mock library.
target_include_directories(telemetry_msgsender
    PUBLIC
        $<BUILD_INTERFACE:${T2_HEADER_DIR}>)

# Alias so that target_link_libraries(... telemetry_msgsender::telemetry_msgsender)
# works when the mock is pulled in via add_subdirectory().
add_library(telemetry_msgsender::telemetry_msgsender ALIAS telemetry_msgsender)

# Install the library so it ends up on the target filesystem
include(GNUInstallDirs)
install(TARGETS telemetry_msgsender
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT ${NAMESPACE}${PROJECT_NAME})
