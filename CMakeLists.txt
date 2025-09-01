cmake_minimum_required(VERSION 3.14)
project(ExchangeBackend VERSION 1.0 LANGUAGES CXX)

# Require at least C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(exchange_backend apps/exchange_server.cpp)

# Include project headers
target_include_directories(exchange_backend PRIVATE ${CMAKE_SOURCE_DIR}/include)

# ASIO standalone
target_compile_definitions(exchange_backend PRIVATE ASIO_STANDALONE)

# Threads for ASIO io_context
find_package(Threads REQUIRED)
target_link_libraries(exchange_backend PRIVATE Threads::Threads)
