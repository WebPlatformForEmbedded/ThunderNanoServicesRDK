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

cmake_minimum_required(VERSION 3.15)

project(telemetry_msgsender_mock C)

add_library(telemetry_msgsender SHARED t2_mock.c)

# Expose the mock header so that #include <telemetry_busmessage_sender.h>
# resolves when building against the mock library.
target_include_directories(telemetry_msgsender
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

# Alias so that target_link_libraries(... telemetry_msgsender::telemetry_msgsender)
# works when the mock is pulled in via add_subdirectory().
add_library(telemetry_msgsender::telemetry_msgsender ALIAS telemetry_msgsender)

# Install the library so it ends up on the target filesystem
include(GNUInstallDirs)
install(TARGETS telemetry_msgsender
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT ${NAMESPACE}${PROJECT_NAME})
