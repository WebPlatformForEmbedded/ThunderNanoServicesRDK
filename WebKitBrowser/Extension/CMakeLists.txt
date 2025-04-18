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

set(PLUGIN_NAME Extension)
set(MODULE_NAME WPE${PLUGIN_NAME})

find_package(${NAMESPACE}Plugins REQUIRED)
find_package(securityagent QUIET)
find_package(WPEWebKit REQUIRED)

option(PLUGIN_SECURITYAGENT "Enable the Security Agent Features interface in javascript." OFF)
option(PLUGIN_WEBKITBROWSER_IIDENTIFIER "Enable IIDENTIFIER interface JS bindings." OFF)
option(PLUGIN_WEBKITBROWSER_AAMP_JSBINDINGS "Enable AAMP JS bindings." OFF)
option(PLUGIN_WEBKITBROWSER_BADGER_BRIDGE "Enable $badger support." OFF)
option(PLUGIN_WEBKITBROWSER_UPDATE_TZ_FROM_FILE "Update TimeZone from given file" OFF)
option(PLUGIN_WEBKITBROWSER_CUSTOM_PROCESS_INFO "Enable set of custom process info (e.g. process name)." OFF)

add_library(${MODULE_NAME} SHARED
    main.cpp
    WhiteListedOriginDomainsList.cpp
    Milestone.cpp
    NotifyThunder.cpp
    RequestHeaders.cpp
    ../Tags.cpp
)

if(securityagent_FOUND)
    target_sources(${MODULE_NAME} PRIVATE SecurityAgent.cpp)
    target_compile_definitions(${MODULE_NAME} PRIVATE ENABLE_SECURITY_AGENT)
    target_link_libraries(${MODULE_NAME} PRIVATE securityagent::securityagent)
endif()

if(PLUGIN_WEBKITBROWSER_IIDENTIFIER)
    target_sources(${MODULE_NAME} PRIVATE IIdentifier.cpp)
    target_compile_definitions(${MODULE_NAME} PRIVATE ENABLE_IIDENTIFIER)
endif()

if(PLUGIN_WEBKITBROWSER_AAMP_JSBINDINGS)
    find_package(AampJSBindings REQUIRED)
    target_sources(${MODULE_NAME} PRIVATE AAMPJSBindings.cpp)
    target_compile_definitions(${MODULE_NAME} PRIVATE ENABLE_AAMP_JSBINDINGS)
    target_link_libraries(${MODULE_NAME} PRIVATE ${AAMP_JSBINDINGS_LIBRARIES})
endif()

if(PLUGIN_WEBKITBROWSER_BADGER_BRIDGE)
    target_sources(${MODULE_NAME} PRIVATE BridgeObject.cpp)
    target_compile_definitions(${MODULE_NAME} PRIVATE ENABLE_BADGER_BRIDGE)
endif()

# This is a Comcast only feature, and for a modular architecture (separation of concern)
# should use the TimeSync plugin
if(PLUGIN_WEBKITBROWSER_UPDATE_TZ_FROM_FILE)
    target_sources(${MODULE_NAME} PRIVATE ../TimeZone/TimeZoneSupport.cpp)
    target_compile_definitions(${MODULE_NAME} PRIVATE UPDATE_TZ_FROM_FILE)
    target_include_directories(${MODULE_NAME} PRIVATE ../TimeZone)
    target_include_directories(${MODULE_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
    if(TZ_FILE)
        add_definitions(-DTZ_FILE="${TZ_FILE}")
    endif()
endif()

if(PLUGIN_WEBKITBROWSER_CUSTOM_PROCESS_INFO)
    target_compile_definitions(${MODULE_NAME} PRIVATE ENABLE_CUSTOM_PROCESS_INFO)
endif()

if (PLUGIN_WEBKITBROWSER_TESTING)
    target_compile_definitions(${MODULE_NAME} PRIVATE ENABLE_TESTING)
    target_include_directories(${MODULE_NAME} PRIVATE ../Testing/Extension)
    target_sources(${MODULE_NAME} PRIVATE ../Testing/Extension/testrunnerextension.cpp)
endif()

set_target_properties(${MODULE_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES)

target_compile_options (${MODULE_NAME} PRIVATE -Wno-psabi)

target_link_libraries(${MODULE_NAME}
    PRIVATE
        ${NAMESPACE}Plugins::${NAMESPACE}Plugins
        ${NAMESPACE}Core::${NAMESPACE}Core
        WPEWebKit::WPEWebKit)

install(TARGETS ${MODULE_NAME}
    PERMISSIONS OWNER_READ GROUP_READ
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${NAMESPACE}/WebKitBrowser/${PLUGIN_WEBKITBROWSER_EXTENSION_DIRECTORY} COMPONENT ${NAMESPACE}_Runtime)
