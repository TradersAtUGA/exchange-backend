add_library(config_parser
    yaml_parser.cpp
)

target_include_directories(config_parser
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include
)

find_package(yaml-cpp CONFIG REQUIRED)

target_link_libraries(config_parser
    PRIVATE yaml-cpp
)