#include <yaml-cpp/yaml.h>
#include <string>
#include "../../include/config-parser/yaml_parser.hpp"


namespace config {


YAMLConfig::YAMLConfig(const std::string& file) {
    config = YAML::LoadFile(file);
}

uint64_t YAMLConfig::get(const std::vector<std::string>& keys) {
    YAML::Node node = this->config;
    for (auto& key : keys) {
        node = node[key];
    }
    return node.as<uint64_t>();
}

}