add_library(network-inbound
    api_routing.cpp
    data_conversion.cpp
    data_validation.cpp
)

target_include_directories(network-inbound
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# If this module needs compile definitions (like ASIO standalone)
# target_compile_definitions(network-inbound
#     PUBLIC ASIO_STANDALONE
# )

# If it depends on external libs find them here
find_package(Threads REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(concurrentqueue CONFIG REQUIRED)

# link them here
target_link_libraries(network-inbound
    PRIVATE Threads::Threads Crow::Crow asio::asio concurrentqueue::concurrentqueue
)
