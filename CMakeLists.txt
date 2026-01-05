cmake_minimum_required(VERSION 3.22)
project(exchange_backend VERSION 1.0 LANGUAGES CXX)

# define DEBUG macro for all targets in project when in debug mode
add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

# Require at least C++20
set(CMAKE_CXX_STANDARD           20)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

if (NOT CMAKE_BUILD_TYPE)
  message("--Setting build type to 'Debug' as none was specified")
  set(CMAKE_BUILD_TYPE "Debug" FORCE)
endif()

#
## Unit Tests
#option(BUILD_TESTING "Build unit tests" ON)
#
#if(BUILD_TESTING)
#    enable_testing()
#    # Add all test subdirectories here
#    add_subdirectory(tests/unit/network-inbound)
#endif()
#

add_subdirectory(src/engine)
add_subdirectory(src/exchange)
add_subdirectory(src/sequencer)
add_subdirectory(src/network-inbound)
# add_subdirectory(src/network-outbound)
add_subdirectory(src/network-manager)


add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        exchange_controller
)
