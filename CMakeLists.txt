cmake_minimum_required(VERSION 3.14)
project(ExchangeBackend VERSION 1.0 LANGUAGES CXX)

# Require at least C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Get current time in MM-DD-YYYY HH:MM:SS format
string(TIMESTAMP CURRENT_TIME "%m-%d-%Y %H:%M:%S")

# -------------------------------
# Global compile-time constants
# To change the const value, change here
# -------------------------------
# Network
set(NETWORK_INBOUND_PORT_NUMBER 11000)
set(NETWORK_INBOUND_THREAD_COUNT 4)
set(NETWORK_OUTBOUND_PORT_NUMBER 12000)
set(NETWORK_OUTBOUND_THREAD_COUNT 4)

# Ring Buffer
set(RING_BUFFER_SIZE 1024)

# Tickers
set(TICKERS "\"ABC\", \"XYZ\", \"QQQ\"")

configure_file(
    ${CMAKE_SOURCE_DIR}/templates/ticker.hpp.in
    ${CMAKE_BINARY_DIR}/generated/include/ticker.hpp
    @ONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/templates/config.hpp.in
    ${CMAKE_BINARY_DIR}/generated/include/config.hpp
    @ONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/templates/run.sh.in
    ${CMAKE_BINARY_DIR}/generated/scripts/run.sh
    @ONLY
)

# -------------------------------
# Cmake Tests
# -------------------------------
option(BUILD_TESTING "Build unit tests" ON)

if(BUILD_TESTING)
    enable_testing()
    # Add all test subdirectories here
    add_subdirectory(tests/unit/network-inbound)
endif()


# -------------------------------
# All src/ subdirectories
# -------------------------------
add_subdirectory(src/network-inbound)
add_subdirectory(src/exchange)
add_subdirectory(src/sequencer)
add_subdirectory(src/engine)

# uncomment these later when implemented AND contain CMAKE file
# add_subdirectory(src/network-outbound)
# add_subdirectory(src/matching-engine)

# -------------------------------
# All executables
# -------------------------------
add_subdirectory(apps/exchange-server)


