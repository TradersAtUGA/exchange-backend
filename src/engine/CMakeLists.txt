add_library(engine
    matching_engine.cpp
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/include/
        @ONLY
)