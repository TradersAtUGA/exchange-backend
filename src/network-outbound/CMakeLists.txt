add_library(network_inbound

# add files here
)

target_include_directories(network_inbound
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/include/
        @ONLY
)

