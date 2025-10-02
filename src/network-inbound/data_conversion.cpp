#include "../../include/network-inbound/data_conversion.hpp"
#include "../../include/order.hpp"
#include "crow.h"
#include <cstdint>
#include <string>

uint64_t cast_json_value(const crow::json::rvalue& json, const std::string& json_parameter) {
    return static_cast<uint64_t>(json[json_parameter].i());
    // TODO catch exception if something goes wrong
}

Side get_side(const crow::json::rvalue& json) {
    std::string side = json["side"].s();
    // bug if side == "anythong but buy" this returns Sell
    return (side == "buy") ? Side::Buy : Side::Sell;
}

OrderType get_order_type(const crow::json::rvalue& json) {
    std::string order_type = json["type"].s();
    // Same bug here
    return (order_type == "market") ? OrderType::Market : OrderType::Limit;
}

namespace exchange::network_inbound {

exchange::Order json_to_exchange_order(const crow::json::rvalue& json) {
    return exchange::Order(
        cast_json_value(json, "client_id"),
        cast_json_value(json, "order_id"),
        get_side(json),
        get_order_type(json),
        cast_json_value(json, "price"),
        cast_json_value(json, "quantity"),
        cast_json_value(json, "remaining"),
        cast_json_value(json, "timestamp")
    );
} 

} // network_inbound