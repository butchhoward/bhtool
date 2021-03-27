# Testing library
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0
)
FetchContent_MakeAvailable(googletest)
# ${gtest_build_include_dirs}

# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.

# There can be multiple test programs
# Just add sets of these four commands
add_executable( testbhtool
                test_bhtool.cpp
                test_stderrred.cpp
                test_repo.cpp
              )
target_compile_features(testbhtool PRIVATE cxx_std_20)
target_link_libraries(testbhtool PRIVATE gtest_main
                                      bhtool_library
                                      )

add_test(NAME testbhtooltest COMMAND testbhtool)


