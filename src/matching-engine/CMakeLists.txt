add_library(matching_engine)

target_sources(matching_engine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/matching_engine.cpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/orderbook.cpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/sequencer.cpp 
)

target_include_directories(matching_engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_precompile_headers(matching_engine PRIVATE pch.h)

#sequencer stuff
find_package(concurrentqueue CONFIG REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(sequencer
    PRIVATE concurrentqueue::concurrentqueue Threads::Threads
)


