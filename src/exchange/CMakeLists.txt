add_library(exchange
    exchange.cpp
)

target_include_directories(exchange
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/include/
        @ONLY
)

find_package(concurrentqueue CONFIG REQUIRED)

target_link_libraries(exchange
    concurrentqueue::concurrentqueue
)