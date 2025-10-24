add_library(exchange
    exchange.cpp
    ../sequencer/sequencer.cpp      
    ../engine/matching_engine.cpp   
)

target_include_directories(exchange
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/include/
        @ONLY
)

find_package(concurrentqueue CONFIG REQUIRED)

target_link_libraries(exchange
    concurrentqueue::concurrentqueue
)