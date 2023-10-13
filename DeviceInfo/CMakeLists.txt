# If not stated otherwise in this file or this component's LICENSE file the
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

project(DeviceInfo)

cmake_minimum_required(VERSION 3.3)

find_package(WPEFramework)

project_version(1.0.0)

set(MODULE_NAME ${NAMESPACE}${PROJECT_NAME})

message("Setup ${MODULE_NAME} v${PROJECT_VERSION}")

set(PLUGIN_DEVICEINFO_AUTOSTART "true" CACHE STRING "Automatically start DeviceInfo plugin")
set(PLUGIN_DEVICEINFO_MODE "Off" CACHE STRING "Controls if the plugin should run in its own process, in process or remote")
set(PLUGIN_DEVICEINFO_STARTUPORDER "" CACHE STRING "Start-up order for DeviceInfo plugin")

find_package(${NAMESPACE}Plugins REQUIRED)
find_package(${NAMESPACE}Definitions REQUIRED)
find_package(CompileSettingsDebug CONFIG REQUIRED)

add_library(${MODULE_NAME} SHARED
    DeviceInfo.cpp
    DeviceInfoJsonRpc.cpp
    Module.cpp)

set_target_properties(${MODULE_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES)

target_link_libraries(${MODULE_NAME}
PRIVATE
    CompileSettingsDebug::CompileSettingsDebug
    ${NAMESPACE}Definitions::${NAMESPACE}Definitions
    ${NAMESPACE}Plugins::${NAMESPACE}Plugins)

if (USE_DEVICESETTINGS)
    find_package(DS REQUIRED)
    find_package(IARMBus REQUIRED)
    find_package(RFC REQUIRED)

    target_include_directories(${MODULE_NAME} PRIVATE
        ${RFC_INCLUDE_DIRS}
        ${DS_INCLUDE_DIRS}
        ${IARMBUS_INCLUDE_DIRS}
        ../helpers)

    target_link_libraries(${MODULE_NAME} PRIVATE
        ${NAMESPACE}Plugins::${NAMESPACE}Plugins
        ${IARMBUS_LIBRARIES}
        ${DS_LIBRARIES})

    target_sources(${MODULE_NAME}
    PRIVATE
        Implementation/DeviceSettings/DeviceInfoImplementation.cpp)

    set_source_files_properties(
        Implementation/DeviceSettings/DeviceInfoImplementation.cpp
        PROPERTIES
        COMPILE_FLAGS "-fexceptions")

    target_compile_definitions(${MODULE_NAME}
    PRIVATE
        USE_DEVICESETTINGS=1)

else ()
    target_sources(${MODULE_NAME}
    PRIVATE
        Implementation/Generic/DeviceInfoImplementation.cpp)
endif ()


install(TARGETS ${MODULE_NAME}
    DESTINATION lib/${STORAGE_DIRECTORY}/plugins)

write_config()
