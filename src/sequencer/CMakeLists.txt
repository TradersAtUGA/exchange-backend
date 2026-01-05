add_library(sequencer STATIC sequencer.cpp)

target_include_directories(sequencer
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

find_package(concurrentqueue CONFIG REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(sequencer
    PRIVATE concurrentqueue::concurrentqueue Threads::Threads
)
