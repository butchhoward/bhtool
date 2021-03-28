# - Try to find the ssh2 library
# Once done this will define
#
#  LIBSSH2_FOUND - System has libSSH2
#  LIBSSH2_INCLUDE_DIR - The libSSH2 include directory
#  LIBSSH2_LIBRARIES - The libraries needed to use libSSH2
#  LIBSSH2_DEFINITIONS - Compiler switches required for using libSSH2


# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
#FIND_PACKAGE(PkgConfig)
#PKG_SEARCH_MODULE(PC_LIBSSH2 libSSH2)

SET(LIBSSH2_DEFINITIONS ${PC_LIBSSH2_CFLAGS_OTHER})

FIND_PATH(LIBSSH2_INCLUDE_DIR NAMES libssh2.h
   HINTS
   ${PC_LIBSSH2_INCLUDEDIR}
   ${PC_LIBSSH2_INCLUDE_DIRS}
)

FIND_LIBRARY(LIBSSH2_LIBRARIES NAMES libssh2.a
   HINTS
   ${PC_LIBSSH2_LIBDIR}
   ${PC_LIBSSH2_LIBRARY_DIRS}
)


INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(libssh2 DEFAULT_MSG LIBSSH2_LIBRARIES LIBSSH2_INCLUDE_DIR)

MARK_AS_ADVANCED(LIBSSH2_INCLUDE_DIR LIBSSH2_LIBRARIES)
