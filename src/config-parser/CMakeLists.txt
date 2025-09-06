add_library(config-parser
    yaml_parser.cpp
)

target_include_directories(config-parser
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include
)

find_package(yaml-cpp CONFIG REQUIRED)

target_link_libraries(config-parser
    PRIVATE yaml-cpp
)