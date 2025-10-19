add_library(sequencer
    sequencer.cpp
)

target_include_directories(sequencer
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/include/
        @ONLY
)

find_package(concurrentqueue CONFIG REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(sequencer
    PRIVATE concurrentqueue::concurrentqueue Threads::Threads
)