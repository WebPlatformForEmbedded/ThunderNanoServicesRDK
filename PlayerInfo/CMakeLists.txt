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

project(PlayerInfo)

cmake_minimum_required(VERSION 3.15)

find_package(Thunder)

project_version(1.0.0)

set(MODULE_NAME ${NAMESPACE}${PROJECT_NAME})

message("Setup ${MODULE_NAME} v${PROJECT_VERSION}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

option(DOLBY_SUPPORT "Enable dolby support" OFF)
set(PLUGIN_PLAYERINFO_STARTMODE "Activated" CACHE STRING "Automatically start PlayerInfo plugin")
set(PLUGIN_PLAYERINFO_STARTUPORDER "" CACHE STRING "Automatically start PlayerInfo plugin")
set(PLUGIN_PLAYERINFO_MODE "Off" CACHE STRING "Controls if the plugin should run in its own process, in process or remote")

if(BUILD_REFERENCE)
    add_definitions(-DBUILD_REFERENCE=${BUILD_REFERENCE})
endif()

find_package(${NAMESPACE}Plugins REQUIRED)
find_package(${NAMESPACE}Definitions REQUIRED)
find_package(CompileSettingsDebug CONFIG REQUIRED)

add_library(${MODULE_NAME} SHARED
    PlayerInfo.cpp
    Module.cpp)

set_target_properties(${MODULE_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES)

target_link_libraries(${MODULE_NAME}
    PRIVATE
        CompileSettingsDebug::CompileSettingsDebug
        ${NAMESPACE}Definitions::${NAMESPACE}Definitions
        ${NAMESPACE}Plugins::${NAMESPACE}Plugins)

find_package(GStreamer REQUIRED)
if (GStreamer_FOUND)
    set(PLUGIN_IMPLEMENTATION_GSTREAMER "ON" CACHE STRING "Flag to identify gstreamer implemenation is selected")
    target_link_libraries(${MODULE_NAME}
        PRIVATE
            GStreamer::GStreamer)

    if (USE_DEVICESETTINGS)
        find_package(DS REQUIRED)
        find_package(IARMBus REQUIRED)
        target_include_directories(${MODULE_NAME} PRIVATE
            ${IARMBUS_INCLUDE_DIRS}
            ${DS_INCLUDE_DIRS}
            ../helpers)
        target_link_libraries(${MODULE_NAME} PRIVATE
            ${NAMESPACE}Plugins::${NAMESPACE}Plugins
            ${IARMBUS_LIBRARIES}
            ${DS_LIBRARIES})
        target_sources(${MODULE_NAME}
            PRIVATE
                DeviceSettings/PlatformImplementation.cpp)
    else ()
        target_sources(${MODULE_NAME}
            PRIVATE
                GStreamer/PlatformImplementation.cpp)
    endif ()
endif ()

if (DOLBY_SUPPORT)
    target_sources(${MODULE_NAME}
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/Dolby/DolbyOutput.cpp)

    target_include_directories(${MODULE_NAME}
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/Dolby/include)

    target_compile_definitions(${MODULE_NAME}
        PRIVATE
            DOLBY_SUPPORT=1)

    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Dolby)

    target_link_libraries(${MODULE_NAME}
        PRIVATE
            PlayerInfoDolby)
endif()

install(TARGETS ${MODULE_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/${STORAGE_DIRECTORY}/plugins COMPONENT ${NAMESPACE}_Runtime)

write_config()
