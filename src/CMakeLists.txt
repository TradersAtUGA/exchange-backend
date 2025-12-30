add_executable(exchange_backend main.cpp)

target_link_libraries(exchange_backend
    PRIVATE
    network_manager
    network_inbound
    sequencer
    exchange
    engine
)

# external libraries    
find_package(Crow CONFIG REQUIRED)
find_package(concurrentqueue CONFIG REQUIRED)

target_link_libraries(exchange_backend
    PRIVATE 
    Crow::Crow 
    concurrentqueue::concurrentqueue
)