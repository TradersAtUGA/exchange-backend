add_executable(test_comp test_compile.cpp)

find_package(quickfix CONFIG REQUIRED)

target_link_libraries(test_comp PRIVATE
    matching_engine
    gateway
    quickfix
    outbound
)
