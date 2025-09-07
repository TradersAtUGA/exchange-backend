#pragma once
#include <cstdint>
#include "../order.hpp"
#include "crow.h"
#include <string>

/**
 * @brief converts a decimal value in the user order JSON into a uint64_t
 * 
 * @param json the JSON received from the user to parse
 * @param json_parameter the section of the JSON to convert into a uint64_t
 * @return uint64_t the associated value as a uint64_t of the json_parameter parameter
 * from the given JSON
 */
uint64_t cast_json_value(const crow::json::rvalue& json, const std::string& json_parameter);

/**
 * @brief gets the side of a order JSON
 * 
 * @param json the JSON received from the user to parse
 * @return Side the order side, either buy or sell
 */
Side get_side(const crow::json::rvalue& json);

/**
 * @brief gets the order type of a order JSON
 * 
 * @param json the JSON received from the user to parse
 * @return OrderType the order type of the given order, either market or limit
 */
OrderType get_order_type(const crow::json::rvalue& json);

namespace exchange::network_inbound {

/**
 * @brief Converts a user order JSON to a exchange::Order object
 * 
 * @param json the JSON payload received over network
 * @return exchange::Order The users order as a object representation
 */
exchange::Order json_to_exchange_order(const crow::json::rvalue& json);

}