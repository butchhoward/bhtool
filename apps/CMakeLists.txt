# The can be multiple apps
# Add sets of these three commands
# find_library(SECURITY_FRAMEWORK Security)
# message("-->>> ${SECURITY_FRAMEWORK_LIBRARIES}")
# find_library(CORE_FRAMEWORK CoreFoundation)
# message("-->>> ${CORE_FRAMEWORK_LIBRARIES}")

add_executable(${CMAKE_PROJECT_NAME} bhtool_app.cpp)
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_20)
set(CMAKE_EXE_LINKER_FLAGS
    "-framework CoreFoundation -framework Security"
    )
target_link_libraries(${CMAKE_PROJECT_NAME}
                        PRIVATE
                        bhtool_library
                        fmt::fmt
                        ${LIBGIT2_LIBRARIES}
                        "/opt/homebrew/Cellar/curl/7.75.0/lib/libcurl.a"
                        libz.a
                        libiconv.a
                        ${LIBSSH2_LIBRARIES}
                        "/opt/homebrew/Cellar/openssl@1.1/1.1.1k/lib/libssl.a"
                        "/opt/homebrew/Cellar/openssl@1.1/1.1.1k/lib/libcrypto.a"
                    )
