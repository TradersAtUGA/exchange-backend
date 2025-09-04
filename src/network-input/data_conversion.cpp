#include "../../include/data_conversion.hpp"
#include "../../include/order.hpp"
#include "crow.h"
#include <cstdint>
#include <string>


uint64_t cast_json_value(const crow::json::rvalue& json, const string& json_parameter) {
    return static_cast<uint64_t>(json[json_parameter].i())
    // TODO catch exception if something goes wrong
}



namespace network_input {

    exchange::Order json_to_exchange_order(const crow::json::rvalue& json) {
        return exchange::Order order(
            cast_json_value(json, "client_id"),
            cast_json_value(json, "order_id"),
            cast_json_value(json, "side"), // this does not need to be a uint it should be the enum from order hpp
            cast_json_value(json, "type"), // same applies here
            cast_json_value(json, "price"),
            cast_json_value(json, "quantity"),
            cast_json_value(json, "remaining"),
            cast_json_value(json, "timestamp")
        )
    } 

}