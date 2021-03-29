# - Try to find the ssh2 library
# Once done this will define
#
#  LIBCRYPTO_FOUND - System has LIBCRYPTO
#  LIBCRYPTO_INCLUDE_DIR - The LIBCRYPTO include directory
#  LIBCRYPTO_LIBRARIES - The libraries needed to use LIBCRYPTO
#  LIBCRYPTO_DEFINITIONS - Compiler switches required for using LIBCRYPTO


# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
#FIND_PACKAGE(PkgConfig)
#PKG_SEARCH_MODULE(PC_LIBCRYPTO LIBCRYPTO)

# this is almost certainly wrong for the general case, but that's where it is on this box
set( LIBCRYPTO_PATH "${HOMEBREW_BASE_FOLDER}/Cellar/openssl@1.1/1.1.1k/")

SET(LIBCRYPTO_DEFINITIONS ${PC_LIBCRYPTO_CFLAGS_OTHER})

FIND_PATH(LIBCRYPTO_INCLUDE_DIR NAMES crypto.h
   HINTS
   ${PC_LIBCRYPTO_INCLUDEDIR}
   ${PC_LIBCRYPTO_INCLUDE_DIRS}
   PATHS ${LIBCRYPTO_PATH}
   PATH_SUFFIXES include include/openssl
)

FIND_LIBRARY(LIBCRYPTO_LIBRARIES NAMES libcrypto.a
   HINTS
   ${PC_LIBCRYPTO_LIBDIR}
   ${PC_LIBCRYPTO_LIBRARY_DIRS}
   PATHS ${LIBCRYPTO_PATH}
   PATH_SUFFIXES lib
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(libcrypto DEFAULT_MSG LIBCRYPTO_LIBRARIES LIBCRYPTO_INCLUDE_DIR)

MARK_AS_ADVANCED(LIBCRYPTO_INCLUDE_DIR LIBCRYPTO_LIBRARIES)
