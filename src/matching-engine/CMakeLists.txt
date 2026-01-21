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

target_precompile_headers(matching_engine PRIVATE ${CMAKE_SOURCE_DIR}/pch.h)

# external libraries
find_package(concurrentqueue CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(matching_engine
    PRIVATE 
        concurrentqueue::concurrentqueue 
        Threads::Threads 
        nlohmann_json
)


