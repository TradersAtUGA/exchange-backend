#include <yaml-cpp/yaml.h>
#include <string>
#include "../../include/config-parsing/yaml_parser.hpp"


class YAMLConfig {
private:

    YAML::Node config;

public:

    explicit YAMLConfig(const std::string& file) {
        config = YAML::LoadFile(file);
    }

    uint64_t get(const std::vector<std::string>& keys) {
        YAML::Node node = config;
        for (auto& key : keys) {
            node = node[key];
        }
        return node.as<uint64_t>();
    }
}; // YAMLConfig
