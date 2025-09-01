cmake_minimum_required(VERSION 3.14)
project(MyProject VERSION 1.0 LANGUAGES CXX)

# Require at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MyApp main.cpp)

# Include project headers
target_include_directories(MyApp PRIVATE include)

# Include vcpkg-installed headers (ASIO, Boost.Beast)
set(VCPKG_INSTALLED_DIR "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux")
target_include_directories(MyApp PRIVATE ${VCPKG_INSTALLED_DIR}/include)

# ASIO standalone
target_compile_definitions(MyApp PRIVATE ASIO_STANDALONE)

# Threads for ASIO io_context
find_package(Threads REQUIRED)
target_link_libraries(MyApp PRIVATE Threads::Threads)
