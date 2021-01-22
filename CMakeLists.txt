# If not stated otherwise in this file or this component's license file the
# following copyright and licenses apply:
#
# Copyright 2020 RDK Management
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

project(ThunderNanoServicesRDK)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(NAMESPACE ${PROJECT_NAME} CACHE STRING "Namespace of the project")

message(STATUS "Setting up ${PROJECT_NAME} v${VERSION}")

if (BUILD_REFERENCE)
    add_definitions (-DBUILD_REFERENCE=${BUILD_REFERENCE})
endif()

find_package(WPEFramework)

option(PLUGIN_DEVICEIDENTIFICATION "Include DeviceIdentification plugin" OFF)
option(PLUGIN_DEVICEINFO "Include DeviceInfo plugin" OFF)
option(PLUGIN_LOCATIONSYNC "Include LocationSync plugin" OFF)
option(PLUGIN_MESSENGER "Include Messenger plugin" OFF)
option(PLUGIN_MONITOR "Include Monitor plugin" OFF)
option(PLUGIN_OPENCDMI "Include OpenCDMi plugin" OFF)
option(PLUGIN_PACKAGER "Include Packager plugin" OFF)
option(PLUGIN_SECURITYAGENT "Include SecurityAgent plugin" OFF)
option(PLUGIN_TRACECONTROL "Include TraceControl plugin" OFF)
option(PLUGIN_WEBKITBROWSER "Include WebKitBrowser plugin" OFF)

if(PLUGIN_DEVICEIDENTIFICATION)
    add_subdirectory(DeviceIdentification)
endif()

if(PLUGIN_DEVICEINFO)
    add_subdirectory(DeviceInfo)
endif()

if(PLUGIN_LOCATIONSYNC)
    add_subdirectory(LocationSync)
endif()

if(PLUGIN_MESSENGER)
    add_subdirectory(Messenger)
endif()

if(PLUGIN_MONITOR)
    add_subdirectory(Monitor)
endif()

if(PLUGIN_OPENCDMI)
    add_subdirectory(OpenCDMi)
endif()

if(PLUGIN_PACKAGER)
    add_subdirectory(Packager)
endif()

if(PLUGIN_SECURITYAGENT)
    add_subdirectory(SecurityAgent)
endif()

if(PLUGIN_TRACECONTROL)
    add_subdirectory(TraceControl)
endif()

if(PLUGIN_WEBKITBROWSER)
    add_subdirectory(WebKitBrowser)
endif()
