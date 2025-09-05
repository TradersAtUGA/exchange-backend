add_library(config_parsing
    yaml_parser.cpp
)

target_include_directories(config_parsing
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include
)

find_package(yaml-cpp CONFIG REQUIRED)

target_link_libraries(config_parsing
    PRIVATE yaml-cpp
)