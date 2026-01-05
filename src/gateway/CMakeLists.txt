add_library(gateway STATIC
    network_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gateway-inbound/api_routing.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gateway-inbound/data_conversion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gateway-inbound/data_validation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gateway-outbound/network_outbound.cpp
)

target_include_directories(gateway
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

# If this module needs compile definitions (like ASIO standalone)
target_compile_definitions(gateway
    PUBLIC ASIO_STANDALONE
)

# If it depends on external libs, link them here
find_package(Threads REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(concurrentqueue CONFIG REQUIRED)

target_link_libraries(network_manager
    PRIVATE Threads::Threads Crow::Crow asio::asio concurrentqueue::concurrentqueue
)

