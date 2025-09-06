add_executable(exchange_backend exchange_server.cpp)

target_link_libraries(exchange_backend
    PRIVATE
    network_inbound
    config_parser
)

# external libraries    
find_package(Crow CONFIG REQUIRED)
find_package(concurrentqueue CONFIG REQUIRED)

target_link_libraries(exchange_backend
    PRIVATE 
    Crow::Crow 
    concurrentqueue::concurrentqueue
)