add_executable(test test_compile.cpp)

find_package(quickfix CONFIG REQUIRED)

target_link_libraries(test PRIVATE
    matching_engine
    gateway
    orderbook
    quickfix
)
