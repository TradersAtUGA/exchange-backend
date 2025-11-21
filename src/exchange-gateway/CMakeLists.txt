add_library(exchange_gateway
    gateway.cpp
)

target_include_directories(exchange_gateway
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/include/
        @ONLY
)

# If it depends on external libs, link them here
find_package(Threads REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(concurrentqueue CONFIG REQUIRED)
find_package(quickfix CONFIG REQUIRED)

target_link_libraries(exchange_gateway
    PRIVATE Threads::Threads Crow::Crow asio::asio concurrentqueue::concurrentqueue quickfix 
)



