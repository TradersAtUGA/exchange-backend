find_package(GTest REQUIRED)
find_package(Crow REQUIRED)
find_package(concurrentqueue REQUIRED)

add_executable(network_inbound_tests 
api_routing_test.cpp
data_validation_test.cpp
)

target_link_libraries(network_inbound_tests
    PRIVATE
        network_inbound
        GTest::gtest_main
        Crow::Crow
        concurrentqueue::concurrentqueue
)
