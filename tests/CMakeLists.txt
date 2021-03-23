# Testing library
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0
)
FetchContent_MakeAvailable(googletest)
# ${gtest_build_include_dirs}

add_executable(testlib testlib.cpp)
target_compile_features(testlib PRIVATE cxx_std_17)
target_link_libraries(testlib PRIVATE gtest_main
                                      modern_library
                                      )

# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.
add_test(NAME testlibtest COMMAND testlib) # Command can be a target

add_executable(testbhtool testbhtool.cpp)
target_compile_features(testbhtool PRIVATE cxx_std_17)
target_link_libraries(testbhtool PRIVATE gtest_main
                                      bhtool_library
                                      )

add_test(NAME testbhtooltest COMMAND testbhtool)


