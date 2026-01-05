add_library(exchange_controller STATIC exchange_controller.cpp)

target_precompile_headers(exchange_controller PRIVATE ${CMAKE_SOURCE_DIR}/include/pch.hpp)

target_include_directories(exchange_controller
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

find_package(concurrentqueue CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)

target_link_libraries(exchange_controller
    PRIVATE
    concurrentqueue::concurrentqueue
    Crow::Crow
)
