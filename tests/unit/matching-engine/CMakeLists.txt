add_library(matching_engine_tests)

target_sources(matching_engine_tests
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/limit_order_test.cpp
)

find_package(GTest REQUIRED)

target_include_directories(matching_engine_tests PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(matching_engine_tests
    matching_engine
    GTest::gtest_main
)

include(GoogleTest)
