add_executable(app app.cpp)
target_compile_features(app PRIVATE cxx_std_17)
target_link_libraries(app PRIVATE modern_library fmt::fmt)


add_executable(${CMAKE_PROJECT_NAME} bhtool.cpp)
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${CMAKE_PROJECT_NAME}
                        PRIVATE
                        bhtool_library
                        fmt::fmt
                    )
