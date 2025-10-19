add_library(shared
    ring_buffer.cpp
)

target_include_directories(shared
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/include/
        @ONLY
)