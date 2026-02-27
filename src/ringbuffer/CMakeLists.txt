add_library(ring_buffer)

target_sources(ring_buffer
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/shared_memory.cpp
)

target_include_directories(ring_buffer
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_precompile_headers(ring_buffer PRIVATE ${CMAKE_SOURCE_DIR}/pch.h)