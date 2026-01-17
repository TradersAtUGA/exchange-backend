add_library(outbound)

find_package(nlohmann_json CONFIG REQUIRED)

target_sources(outbound
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/private_broadcaster.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/public_broadcaster.cpp
)

target_include_directories(outbound
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(outbound PRIVATE nlohmann_json)