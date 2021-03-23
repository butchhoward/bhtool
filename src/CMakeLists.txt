# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/modern/lib.hpp"
    "${PROJECT_SOURCE_DIR}/include/bhtool/bhtool.hpp"
    "${PROJECT_SOURCE_DIR}/include/bhtool/stderrred.hpp"
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(modern_library lib.cpp ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(modern_library PUBLIC ../include)

# All users of this library will need at least C++17
target_compile_features(modern_library PUBLIC cxx_std_17)


add_library(bhtool_library
            bhtool_lib.cpp
            stderrred.cpp
            ${HEADER_LIST})
target_include_directories(bhtool_library PUBLIC ../include)
target_compile_features(bhtool_library PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
