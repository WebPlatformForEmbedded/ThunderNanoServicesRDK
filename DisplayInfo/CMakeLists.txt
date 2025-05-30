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

project(DisplayInfo)

cmake_minimum_required(VERSION 3.15)

find_package(Thunder)

project_version(1.0.0)

set(MODULE_NAME ${NAMESPACE}${PROJECT_NAME})
message("Setup ${MODULE_NAME} v${PROJECT_VERSION}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(PLUGIN_DISPLAYINFO_STARTMODE "Activated" CACHE STRING "Automatically start DisplayInfo plugin")
set(PLUGIN_DISPLAYINFO_STARTUPORDER "" CACHE STRING "Automatically start DisplayInfo plugin")
set(PLUGIN_DISPLAYINFO_MODE "Off" CACHE STRING "Controls if the plugin should run in its own process, in process or remote.")

# deprecated/legacy flags support
if(PLUGIN_DISPLAYINFO_OUTOFPROCESS STREQUAL "true")
    set(PLUGIN_DISPLAYINFO_MODE "Local" CACHE STRING "Process mode"  FORCE)
    unset(PLUGIN_DISPLAYINFO_OUTOFPROCESS CACHE)
endif()

if(BUILD_REFERENCE)
    add_definitions(-DBUILD_REFERENCE=${BUILD_REFERENCE})
endif()

find_package(${NAMESPACE}Plugins REQUIRED)
find_package(${NAMESPACE}Definitions REQUIRED)
find_package(CompileSettingsDebug CONFIG REQUIRED)
find_package(BCM_HOST QUIET)
find_package(NEXUS QUIET)
find_package(NXCLIENT QUIET)
find_package(libdrm QUIET)

add_library(${MODULE_NAME} SHARED
    DisplayInfo.cpp
    Module.cpp)

set_target_properties(${MODULE_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES)

target_link_libraries(${MODULE_NAME}
    PRIVATE
        CompileSettingsDebug::CompileSettingsDebug
        ${NAMESPACE}Plugins::${NAMESPACE}Plugins
        ${NAMESPACE}Definitions::${NAMESPACE}Definitions)

if (PLUGIN_DISPLAYINFO_BCM_VERSION_MAJOR)
    target_compile_definitions(${MODULE_NAME}
        PRIVATE
            DISPLAYINFO_BCM_VERSION_MAJOR=${PLUGIN_DISPLAYINFO_BCM_VERSION_MAJOR})
endif()

if (USE_DEVICESETTINGS)
    find_package(DS REQUIRED)
    find_package(IARMBus REQUIRED)
    target_include_directories(${MODULE_NAME} PRIVATE ${IARMBUS_INCLUDE_DIRS}
        ${DS_INCLUDE_DIRS}
        ../helpers)
    target_link_libraries(${MODULE_NAME} PRIVATE
        ${NAMESPACE}Plugins::${NAMESPACE}Plugins
        ${IARMBUS_LIBRARIES}
        ${DS_LIBRARIES})
    target_sources(${MODULE_NAME}
        PRIVATE
            DeviceSettings/PlatformImplementation.cpp)
    if (BUILD_BROADCOM)
        target_sources(${MODULE_NAME}
            PRIVATE
                DeviceSettings/Broadcom/SoC_abstraction.cpp)
    elseif (BUILD_REALTEK)
        target_sources(${MODULE_NAME}
            PRIVATE
                DeviceSettings/Realtek/SoC_abstraction.cpp
                DeviceSettings/Realtek/kms.c)
    elseif (BUILD_AMLOGIC)
        target_sources(${MODULE_NAME}
            PRIVATE
            DeviceSettings/Amlogic/SoC_abstraction.cpp
            DeviceSettings/Amlogic/kms.c)
    endif()

elseif (NXCLIENT_FOUND AND NEXUS_FOUND)
    if (DISPLAYINFO_IMPLEMENTATION_PATH)
        target_sources(${MODULE_NAME}
            PRIVATE
                ${DISPLAYINFO_IMPLEMENTATION_PATH}/PlatformImplementation.cpp)
    else()
        include(GetExternalCode)
        set(DISPLAYINFO_IMPLEMENTATION_VERSION "master" CACHE STRING "DisplayInfo implementation version: commit_id_or_tag_or_branch_name")
        set(DISPLAYINFO_IMPLEMENTATION_REPOSITORY "https://code.rdkcentral.com/r/soc/broadcom/components/rdkcentral/thundernanoservices/DisplayInfo" CACHE STRING "DisplayInfo implementation repository")
        GetExternalCode(
            GIT_REPOSITORY ${DISPLAYINFO_IMPLEMENTATION_REPOSITORY}
            GIT_VERSION ${DISPLAYINFO_IMPLEMENTATION_VERSION}
            SOURCE_DIR "Nexus"
        )
        target_sources(${MODULE_NAME}
            PRIVATE
                Nexus/PlatformImplementation.cpp)
    endif()
    target_link_libraries(${MODULE_NAME}
        PRIVATE
            NEXUS::NEXUS
            NXCLIENT::NXCLIENT)

elseif (BCM_HOST_FOUND)
    target_sources(${MODULE_NAME}
        PRIVATE
            RPI/PlatformImplementation.cpp)
    target_link_libraries(${MODULE_NAME}
        PRIVATE
            BCM_HOST::BCM_HOST)

elseif (LIBDRM_FOUND)
    target_sources(${MODULE_NAME}
        PRIVATE
	Linux/PlatformImplementation.cpp)

    target_link_libraries(${MODULE_NAME}
        PRIVATE
            libdrm::libdrm)
else ()
    message(FATAL_ERROR "There is no graphic backend for display info plugin")
endif ()

install(TARGETS ${MODULE_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/${STORAGE_DIRECTORY}/plugins COMPONENT ${NAMESPACE}_Runtime)

write_config()
