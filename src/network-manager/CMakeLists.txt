add_library(network-manager
    network_manager.cpp
    ../api_routing.cpp
    ../data_conversion.cpp
    ../data_validation.cpp
)

target_include_directories(exchange
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/include/
        @ONLY
)

