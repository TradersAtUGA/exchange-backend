# find_package(GTest REQUIRED)
# find_package(Crow REQUIRED)
# find_package(concurrentqueue REQUIRED)

# add_executable(network_inbound_tests 
# api_routing_test.cpp
# data_validation_test.cpp
# )

# target_link_libraries(network_inbound_tests
#     PRIVATE
#         network_inbound
#         GTest::gtest_main
#         Crow::Crow
#         concurrentqueue::concurrentqueue
# )

find_package(GTest REQUIRED)
find_package(concurrentqueue REQUIRED)

add_executable(smoke_test api_routing_test.cpp)
target_link_libraries(smoke_test PRIVATE GTest::gtest_main concurrentqueue::concurrentqueue)

add_test(NAME SmokeTest COMMAND smoke_test)
