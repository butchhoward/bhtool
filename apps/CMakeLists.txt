# The can be multiple apps
# Add sets of these three commands
add_executable(${CMAKE_PROJECT_NAME} bhtool_app.cpp)
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${CMAKE_PROJECT_NAME}
                        PRIVATE
                        bhtool_library
                        fmt::fmt
                    )
