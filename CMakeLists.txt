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
project(NanoServicesRDK)

find_package(WPEFramework)

# All packages that did not deliver a CMake Find script (and some deprecated scripts that need to be removed)
# are located in the cmake directory. Include it in the search.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

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

# Library installation location
string(TOLOWER ${NAMESPACE} STORAGE_DIRECTORY)

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

if(WPEFRAMEWORK_CREATE_IPKG_TARGETS)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEB_COMPONENT_INSTALL ON)
    set(CPACK_COMPONENTS_GROUPING IGNORE)

    set(CPACK_DEBIAN_PACKAGE_NAME "${WPEFRAMEWORK_PLUGINS_OPKG_NAME}")
    set(CPACK_DEBIAN_PACKAGE_VERSION "${WPEFRAMEWORK_PLUGINS_OPKG_VERSION}")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${WPEFRAMEWORK_PLUGINS_OPKG_ARCHITECTURE}")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${WPEFRAMEWORK_PLUGINS_OPKG_MAINTAINER}")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${WPEFRAMEWORK_PLUGINS_OPKG_DESCRIPTION}")
    set(CPACK_PACKAGE_FILE_NAME "${WPEFRAMEWORK_PLUGINS_OPKG_FILE_NAME}")

    # list of components from which packages will be generated
    set(CPACK_COMPONENTS_ALL
            ${NAMESPACE}WebKitBrowser
            WPEInjectedBundle
            )

    include(CPack)
endif()