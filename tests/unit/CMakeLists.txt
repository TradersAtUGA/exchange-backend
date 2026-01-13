add_executable(unit_tests main.cpp)

target_include_directories(unit_tests PUBLIC ${CMAKE_SOURCE_DIR}/include)

find_package(GTest REQUIRED)

target_link_libraries(unit_tests PUBLIC
    GTest::gtest_main
    matching_engine_tests
)

gtest_discover_tests(unit_tests)