add_library(network_outbound

# add files here
)

target_include_directories(network_outbound
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/include/
        @ONLY
)

