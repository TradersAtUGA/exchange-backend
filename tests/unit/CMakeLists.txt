add_executable(unit_tests
    matching-engine/limit_order_test.cpp
)

target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

find_package(GTest REQUIRED)

target_link_libraries(unit_tests PRIVATE
    GTest::gtest_main
    matching_engine
)

include(GoogleTest)
gtest_discover_tests(unit_tests)