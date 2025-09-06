cmake_minimum_required(VERSION 3.14)
project(ExchangeBackend VERSION 1.0 LANGUAGES CXX)

# Require at least C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# -------------------------------
# Global compile-time constants
# -------------------------------
# Defaults (can be overridden via -D)
set(NETWORK_INBOUND_PORT_NUMBER 10000 CACHE INTEGER "Port number for inbound network traffic")
set(NETWORK_INBOUND_THREAD_COUNT 4 CACHE INTEGER "Number of worker threads accepting inbound network traffic")
set(NETWORK_OUTBOUND_PORT_NUMBER 12000 CACHE INTEGER "Port number for outbound network traffic")
set(NETWORK_OUTBOUND_THREAD_COUNT 4 CACHE INTEGER "Number of worker threads sending outbound network traffic")

configure_file(
    ${CMAKE_SOURCE_DIR}/config.hpp.in
    ${CMAKE_BINARY_DIR}/config.hpp
)


# -------------------------------
# Cmake Tests add later
# -------------------------------
# option(BUILD_TESTING "Build unit tests" ON)

# if(BUILD_TESTING)
#     enable_testing()
#     add_subdirectory(tests/network-inbound)
# endif()


# -------------------------------
# All src/ subdirectories
# -------------------------------
add_subdirectory(src/network-inbound)
# uncomment these later
# add_subdirectory(src/network-outbound)
# add_subdirectory(src/shared)
# add_subdirectory(src/matching-engine)


# -------------------------------
# All executables
# -------------------------------
add_subdirectory(apps/exchange-server)