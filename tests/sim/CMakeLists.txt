file(GLOB_RECURSE SIM_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(sim_tests ${SIM_SOURCES})

target_include_directories(sim_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(GTest REQUIRED)

target_link_libraries(sim_tests PRIVATE
    GTest::gtest_main
    matching_engine
)

include(GoogleTest)
gtest_discover_tests(sim_tests)
