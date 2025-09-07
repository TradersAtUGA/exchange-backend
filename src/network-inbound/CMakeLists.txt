add_library(network_inbound
    api_routing.cpp
    data_conversion.cpp
    data_validation.cpp
)

target_include_directories(network_inbound
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/include/
        @ONLY
)

# If this module needs compile definitions (like ASIO standalone)
target_compile_definitions(network_inbound
    PUBLIC ASIO_STANDALONE
)

# If it depends on external libs, link them here
find_package(Threads REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(concurrentqueue CONFIG REQUIRED)

target_link_libraries(network_inbound
    PRIVATE Threads::Threads Crow::Crow asio::asio concurrentqueue::concurrentqueue
)
