#pragma once
#include <cstdint>
#include "../order.hpp"
#include "crow.h"

/**
 * @brief converts a decimal value in the user order JSOn into a uint64_t
 * 
 * @param json the json received from the user to parse
 * @param json_parameter the section of the JSON to convert into a uint64_t
 */
uint64_t cast_json_value(const crow::json::rvalue& json, const string& json_parameter);

namespace network_input {

    /**
     * @brief Converts a user order JSON to a exchange::Order object
     * 
     * @param json the JSON payload received over network
     * @return exchange::Order The users order as a object representation
     */
    exchange::Order json_to_exchange_order(const crow::json::rvalue& json);

}