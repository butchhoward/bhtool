# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/bhtool/bhtool.hpp"
    "${PROJECT_SOURCE_DIR}/include/bhtool/spc.hpp"
    "${PROJECT_SOURCE_DIR}/include/bhtool/stderrred.hpp"
    "${PROJECT_SOURCE_DIR}/include/bhtool/color_streams.h"
)

# These can be multiple libraries built here
# Just add sets of these three commands
add_library(bhtool_library
            bhtool_lib.cpp
            spc.cpp
            stderrred.cpp
            ${HEADER_LIST})
target_include_directories(bhtool_library PUBLIC ../include)
target_compile_features(bhtool_library PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
