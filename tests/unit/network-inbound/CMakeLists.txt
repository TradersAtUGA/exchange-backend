add_executable(network_inbound_tests
    api_routing_test.cpp
    data_validation_test.cpp
    data_conversion_test.cpp
)

target_include_directories(network_inbound_tests
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

find_package(GTest REQUIRED)
find_package(concurrentqueue REQUIRED)

target_link_libraries(network_inbound_tests PRIVATE GTest::gtest_main concurrentqueue::concurrentqueue network_inbound)

add_test(NAME network_inbound_tests COMMAND network_inbound_tests)
