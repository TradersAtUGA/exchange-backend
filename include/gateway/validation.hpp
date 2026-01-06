/** @brief validation logic and internal order generation for incoming orders */
#pragma once 

#include <cstdint>
#include <string>
#include <unordered_map>

#include "shared/order.hpp"
#include "shared/enums.hpp"
#include "gateway/gateway.hpp"
#include "shared/utilities.hpp"

namespace exchange {

/*
Dispatch tables to lookup valid values and conversions
into internal enums
*/

const std::unordered_map<char, Side> VALID_SIDES = {
{FIX::Side_BUY, Side::BUY}, 
{FIX::Side_SELL, Side::SELL},
};

const std::unordered_map<char, OrderType> VALID_ORDER_TYPES = {
    {FIX::OrdType_MARKET, OrderType::MARKET},
    {FIX::OrdType_LIMIT, OrderType::LIMIT},
};

const std::unordered_map<char, TIF> VALID_TIF = {
    {FIX::TimeInForce_DAY, TIF::DAY}, 
    {FIX::TimeInForce_IMMEDIATE_OR_CANCEL, TIF::IOC}, 
    {FIX::TimeInForce_FILL_OR_KILL, TIF::FOK},
};

/** @brief looks up in valid side dispatch table if valid side provided */
inline static uint8_t validate_side(char fix_value) {
    
    if (auto it = VALID_SIDES.find(fix_value); it != VALID_SIDES.end()) return true;
    return false;
}

/** @brief looks up in valid order type dispatch table if valid order type was provided */
inline static uint8_t validate_order_type(char fix_value) {
  
    if (auto it = VALID_ORDER_TYPES.find(fix_value); it != VALID_ORDER_TYPES.end()) return true;
    return false;
}

/** @brief looks up in the valid tif dispatch table if valid tif provided */
inline static uint8_t validate_tif(char fix_value) {
     
    if (auto it = VALID_TIF.find(fix_value); it != VALID_TIF.end()) return true;
    return false;
}

/** @brief creates a new order given that its valid */
inline static exchange::Order generate_order(
    char side,
    char order_type, 
    char tif,
    double price,
    double qty,
    std::string client_id
) {
    exchange::Order o; 
    // FIX provided fields 
    o.side = VALID_SIDES.at(side);
    o.order_type = VALID_ORDER_TYPES.at(order_type);
    o.tif = VALID_TIF.at(tif);
    o.price = exchange::double_to_uint64_t(price);
    o.qty = static_cast<uint64_t>(qty); // avoid bit casts for qty
    o.cid = std::stoull(client_id);

    // internally filled fields 
    o.recv_time = exchange::get_time_ns();
    o.status = 1; // order is alive/valid
    o.oid = 0; // TODO: SEQUENCER MUST ASSIGN HERE

    return o;
}

/** @brief validates fix passed values */
inline static uint8_t validate_fix_values(
    char side, 
    char order_type, 
    char tif, 
    double price, 
    double qty
) {

    return validate_side(side)
    && validate_order_type(order_type)
    && validate_tif(tif)
    && static_cast<uint8_t>(price > 0)
    && static_cast<uint8_t>(qty > 0);
}

} // namespace exchange 