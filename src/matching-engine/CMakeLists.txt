add_library(matching_engine STATIC matching_engine.cpp)
target_include_directories(matching_engine PUBLIC ${CMAKE_SOURCE_DIR}/include)