# - Try to find the ssh2 library
# Once done this will define
#
#  LIBCURL_FOUND - System has LIBCURL
#  LIBCURL_INCLUDE_DIR - The LIBCURL include directory
#  LIBCURL_LIBRARIES - The libraries needed to use LIBCURL
#  LIBCURL_DEFINITIONS - Compiler switches required for using LIBCURL


# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
#FIND_PACKAGE(PkgConfig)
#PKG_SEARCH_MODULE(PC_LIBCURL LIBCURL)

# this is almost certainly wrong for the general case, but that's where it is on this box
set(CURL_PATH "/opt/homebrew/Cellar/curl/7.75.0/" )

SET(LIBCURL_DEFINITIONS ${PC_LIBCURL_CFLAGS_OTHER})

FIND_PATH(LIBCURL_INCLUDE_DIR NAMES curl.h
   PATHS ${CURL_PATH}
   PATH_SUFFIXES include include/curl
)

FIND_LIBRARY(LIBCURL_LIBRARIES NAMES libcurl.a
   PATHS ${CURL_PATH}/lib
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(libcurl DEFAULT_MSG LIBCURL_LIBRARIES LIBCURL_INCLUDE_DIR)

MARK_AS_ADVANCED(LIBCURL_INCLUDE_DIR LIBCURL_LIBRARIES)
