#pragma once 
#include <yaml-cpp/yaml.h>
#include <string>
#include <cstdint>
#include <vector>

namespace config {
    
    class YAMLConfig {
    private:

        YAML::Node config;

    public:

        /**
         * @brief creates a new YAMLConfig object
         * @param file the file name as a path that 
         * contains the config information 
         * @return YAMLConfig object
         */
        explicit YAMLConfig(const std::string& file);

        /**
         * @brief gets an attribute from the YAML config 
         * given a list of string keys
         * 
         * @param keys A std::vector containing strings 
         * representing the value needed from the config
         */
        uint64_t get(const std::vector<std::string>& keys); 

    };
}




