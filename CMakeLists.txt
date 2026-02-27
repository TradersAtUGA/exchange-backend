cmake_minimum_required(VERSION 3.25)
project(exchange_backend VERSION 1.0 LANGUAGES CXX)

# # define DEBUG macro for all targets in project when in debug mode
# add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

# Require at least C++20
set(CMAKE_CXX_STANDARD            20)
set(CMAKE_CXX_STANDARD_REQUIRED   ON)
set(CMAKE_CXX_EXTENSIONS          OFF)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Debug' as none was specified")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(ENABLE_LOGGING       "Enable Logging"                      OFF)
option(BUILD_UNIT_TESTING   "Build unit test executable"          OFF)
option(BUILD_PERF_TESTING   "Build performance test executable"   OFF)

if(ENABLE_LOGGING)
  add_compile_definitions(ENABLE_LOGGING)
endif()

if(BUILD_UNIT_TESTING)
  enable_testing()
  add_subdirectory(tests/unit/)
endif()

if(BUILD_PERF_TESTING)
  add_subdirectory(tests/perf/)
endif()


# for src code
add_subdirectory(src/ringbuffer)
add_subdirectory(src/gateway)
add_subdirectory(src/matching-engine)
add_subdirectory(src/outbound)

# for executables
add_subdirectory(apps/)
