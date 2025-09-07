add_executable(network_inbound_performance_tests
    network_inbound_performance_test.cpp
)

target_include_directories(network_inbound_performance_tests
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

find_package(benchmark REQUIRED)

target_link_libraries(network_inbound_performance_tests
    PRIVATE
        benchmark::benchmark
)

add_test(NAME network_inbound_performance_tests COMMAND network_inbound_performance_tests)

