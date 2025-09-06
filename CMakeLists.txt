cmake_minimum_required(VERSION 3.14)
project(ExchangeBackend VERSION 1.0 LANGUAGES CXX)

# Require at least C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTING "Build unit tests" ON)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests/network-inbound)
endif()


# All src/ subdirectories
add_subdirectory(src/network-inbound)
add_subdirectory(src/config-parser)
# add_subdirectory(src/network-outbound)
# add_subdirectory(src/shared)

# All executables
add_subdirectory(apps/exchange-server)