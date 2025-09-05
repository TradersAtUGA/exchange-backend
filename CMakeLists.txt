cmake_minimum_required(VERSION 3.14)
project(ExchangeBackend VERSION 1.0 LANGUAGES CXX)

# Require at least C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# add all files needed to compile the executable
add_executable(exchange_backend 
    apps/exchange_server.cpp
    src/network-inbound/api_routing.cpp
    src/network-inbound/data_validation.cpp
    src/network-inbound/data_conversion.cpp
)

# Include project headers
target_include_directories(exchange_backend PRIVATE ${CMAKE_SOURCE_DIR}/include)

# ASIO standalone
target_compile_definitions(exchange_backend PRIVATE ASIO_STANDALONE)

# 3rd party libraries
# Threads
find_package(Threads REQUIRED)
target_link_libraries(exchange_backend PRIVATE Threads::Threads)

# Crow
find_package(Crow CONFIG REQUIRED)
target_link_libraries(exchange_backend PRIVATE Crow::Crow asio::asio)

# gtest
find_package(GTest REQUIRED)
target_link_libraries(exchange_backend PRIVATE GTest::gtest GTest::gtest_main)

# lock free queue
find_package(concurrentqueue CONFIG REQUIRED)
target_link_libraries(exchange_backend PRIVATE concurrentqueue::concurrentqueue)


