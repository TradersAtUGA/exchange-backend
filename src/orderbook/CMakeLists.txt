add_library(orderbook STATIC orderbook.cpp)
target_include_directories(orderbook PUBLIC ${CMAKE_SOURCE_DIR}/include)