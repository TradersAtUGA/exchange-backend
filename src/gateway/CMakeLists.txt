add_library(gateway STATIC gateway.cpp)

target_include_directories(gateway
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_precompile_headers(gateway PRIVATE pch.h)

find_package(quickfix CONFIG REQUIRED)
target_link_libraries(gateway PRIVATE quickfix)
