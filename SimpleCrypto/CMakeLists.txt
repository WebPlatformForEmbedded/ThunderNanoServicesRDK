# If not stated otherwise in this file or this component's LICENSE file the
# following copyright and licenses apply:
#
# Copyright 2023 RDK Management
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

project(SimpleCrypto)

cmake_minimum_required(VERSION 3.3)

find_package(WPEFramework)
find_package(${NAMESPACE}Plugins REQUIRED)

project_version(1.0.0)

set(MODULE_NAME ${NAMESPACE}${PROJECT_NAME})

message("Setup ${MODULE_NAME} v${PROJECT_VERSION}")

set(PLUGIN_SIMPLECRYPTO_AUTOSTART "true" CACHE STRING "Automatically start the plugin")
set(PLUGIN_SIMPLECRYPTO_MODE "Local" CACHE STRING "Controls if the plugin should run in its own process, in process or remote")

find_package(${NAMESPACE}Plugins REQUIRED)
find_package(CompileSettingsDebug CONFIG REQUIRED)

option(PLUGIN_SIMPLECRYPTO_IMPLEMENTATION_ICRYPTOGRAPHY "Build implementation based on ICryptography" ON)
option(PLUGIN_SIMPLECRYPTO_IMPLEMENTATION_ICRYTPOGRAPHY_USEOPENSSL "Build implementation based on ICryptography/OpenSSL" ON)

add_library(${MODULE_NAME} SHARED
    Module.cpp
    SimpleCrypto.cpp)

if (${PLUGIN_SIMPLECRYPTO_IMPLEMENTATION_ICRYPTOGRAPHY})
    find_package(${NAMESPACE}Cryptography REQUIRED)

    if (${PLUGIN_SIMPLECRYPTO_IMPLEMENTATION_ICRYTPOGRAPHY_USEOPENSSL})
        find_package(OpenSSL REQUIRED)

        target_link_libraries(${MODULE_NAME}
            PRIVATE ssl crypto)

        target_compile_definitions(${MODULE_NAME}
            PRIVATE
                USE_OPENSSL=1)
    else()
        find_package(${NAMESPACE}Cryptalgo REQUIRED)

        target_link_libraries(${MODULE_NAME}
            PRIVATE
                ${NAMESPACE}Cryptalgo::${NAMESPACE}Cryptalgo)
    endif()

    target_sources(${MODULE_NAME}
        PRIVATE
            Implementation/ICryptography/SimpleCryptoImplementation.cpp)

    target_link_libraries(${MODULE_NAME}
        PRIVATE
            ${NAMESPACE}Cryptography::${NAMESPACE}Cryptography)
endif()

set_target_properties(${MODULE_NAME}
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES)

target_compile_options(${MODULE_NAME}
    PRIVATE
        -Wno-psabi
        -fdiagnostics-color=always)

target_link_libraries(${MODULE_NAME} PRIVATE
            CompileSettingsDebug::CompileSettingsDebug
            ${NAMESPACE}Plugins::${NAMESPACE}Plugins)

install(TARGETS ${MODULE_NAME}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${STORAGE_DIRECTORY}/plugins)

write_config()
