# If not stated otherwise in this file or this component's license file the
# following copyright and licenses apply:
#
# Copyright 2020 Metrological B.V.
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

if(LibDRM_FIND_QUIETLY)
    set(_LibDRM_MODE QUIET)
elseif(LibDRM_FIND_REQUIRED)
    set(_LibDRM_MODE REQUIRED)
endif()

find_package(PkgConfig)

# Just check if the libdrm.pc exist, and create the PkgConfig::libdrm target
# No version requirement (yet)
pkg_check_modules(LibDRM ${_LibDRM_MODE} IMPORTED_TARGET libdrm)

include(FindPackageHandleStandardArgs)

# Sets the FOUND variable to TRUE if all required variables are present and set
find_package_handle_standard_args(
    LibDRM
    REQUIRED_VARS
        LibDRM_INCLUDE_DIRS
        LibDRM_CFLAGS
        LibDRM_LDFLAGS
        LibDRM_LIBRARIES
        LibDRM_ACTUAL_LIBRARY
    VERSION_VAR
        LibDRM_VERSION
)
mark_as_advanced(LibDRM_INCLUDE_DIRS LibDRM_LIBRARIES)

if(LibDRM_FOUND)
   add_library(libdrm::libdrm ALIAS PkgConfig::LibDRM)
endif()