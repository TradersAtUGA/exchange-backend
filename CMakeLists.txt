cmake_minimum_required(VERSION 3.22)
project(exchange_backend VERSION 1.0 LANGUAGES CXX)

# define DEBUG macro for all targets in project when in debug mode
add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

# Require at least C++20
set(CMAKE_CXX_STANDARD           20)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Debug' as none was specified")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(BUILD_UNIT_TESTING "Build unit test executable")

if(BUILD_UNIT_TESTING)
  enable_testing()
  add_subdirectory(tests/unit/)
endif()

option(BUILD_PERF_TESTING "Build performance test executable")

if(BUILD_PERF_TESTING)
  add_subdirectory(tests/perf/)
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

add_subdirectory(src/gateway)
add_subdirectory(src/matching-engine)
# add_subdirectory(src/orchestration)

# for executables
add_subdirectory(apps/)

# deprecated
# add_executable(${PROJECT_NAME} src/main.cpp)
#
# target_link_libraries(${PROJECT_NAME}
#     PRIVATE
#         exchange_controller
# )
#
