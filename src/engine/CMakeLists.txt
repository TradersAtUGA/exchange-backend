add_library(engine STATIC 
    matching_engine.cpp
    order_book.cpp
)

target_precompile_headers(engine PRIVATE ${CMAKE_SOURCE_DIR}/include/pch.hpp)

target_include_directories(engine
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)
