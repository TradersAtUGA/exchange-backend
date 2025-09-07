add_executable(network_inbound_tests
    api_routing_test.cpp
    data_validation_test.cpp
)

find_package(GTest REQUIRED)
find_package(concurrentqueue REQUIRED)

target_link_libraries(network_inbound_tests PRIVATE GTest::gtest_main concurrentqueue::concurrentqueue)

add_test(NAME network_inbound_tests COMMAND network_inbound_tests)
