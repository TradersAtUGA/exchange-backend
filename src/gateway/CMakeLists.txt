add_library(gateway STATIC gateway.cpp)

target_include_directories(exchange_gateway
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/include/
        @ONLY
)

find_package(quickfix CONFIG REQUIRED)
target_link_libraries(gateway PRIVATE quickfix)