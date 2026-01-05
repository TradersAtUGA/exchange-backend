add_library(orderbook STATIC orderbook.cpp)

target_include_directories(orderbook
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/include/
        @ONLY
)