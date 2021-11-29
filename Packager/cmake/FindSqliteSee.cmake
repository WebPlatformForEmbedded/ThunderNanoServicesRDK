find_package(PkgConfig)

if(SqliteSee_FIND_QUIETLY)
    set(_SQLITE_SEE_MODE QUIET)
elseif(SqliteSee_FIND_REQUIRED)
    set(_SQLITE_SEE_MODE REQUIRED)
endif()
pkg_search_module(SQLITE_SEE ${_SQLITE_SEE_MODE} sqlite3see)
