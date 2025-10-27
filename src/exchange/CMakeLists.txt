add_library(exchange
    exchange.cpp
    ../sequencer/sequencer.cpp      
    ../engine/matching_engine.cpp 
    ../network-manager/network_manager.cpp  
    ../network-inbound/api_routing.cpp
    ../network-inbound/data_conversion.cpp
    ../network-inbound/data_validation.cpp
)

target_include_directories(exchange
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/generated/include/
        @ONLY
)

find_package(concurrentqueue CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)

target_link_libraries(exchange
    concurrentqueue::concurrentqueue
    Crow::Crow
)