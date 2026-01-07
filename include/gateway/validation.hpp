/** @brief validation logic and internal order generation for incoming orders */
#pragma once 

#include <cstdint>
#include <string>
#include <array>

#include "shared/order.hpp"
#include "shared/enums.hpp"
#include "gateway/gateway.hpp"
#include "shared/utilities.hpp"

namespace exchange {

/*
Dispatch tables to lookup valid values and conversions
into internal enums

Note that fix values are char values and some fix 
valuse begin at '1' or '0'. Depending on indexing 
some arrays are accessed by ['1' - '0' - 1] if they 
start at 1 and ['1' - '0'] if they start at 0. 
*/

// NOTE: fix values start at 1!
inline constexpr std::array<uint8_t, 17> VALID_SIDES = [] {
    std::array<uint8_t, 17> arr; 
    arr[0] = 1; // Side::BUY
    arr[1] = 1; // Side::SELL
    for (size_t i{ 2 }; i < 17; ++i) {
        arr[i] = 0;
    }
    return arr;
}();

// NOTE: fix values DO NOT start at 1!
inline constexpr std::array<uint8_t, 10> VALID_TIF = [] {
    std::array<uint8_t, 10> arr; 
    arr[0] = 1; // TIF::DAY
    arr[1] = 0; // GTC (not impl)
    arr[2] = 0; // at the open (not impl)
    arr[3] = 1; // IOC
    arr[4] = 1; // FOK 
    for (size_t i{ 5 }; i < 10; ++i) {
        arr[i] = 0;
    }
    return arr;
}();

// NOTE: fix values BEGIN at 1!
inline constexpr std::array<uint8_t, 30> VALID_ORDER_TYPES = [] {
    std::array<uint8_t, 30> arr; 
    arr[0] = 1; // market
    arr[1] = 1; // limit
    for (size_t i{ 2 }; i < 30; ++i) {
        arr[i] = 0;
    }
    return arr;
}();


// NOTE: fix values start at 1!
inline constexpr std::array<Side, 2> SIDE_MAP = [] {
    std::array<Side, 2> arr;
    arr[0] = Side::BUY; // FIX::Side_BUY = '1'
    arr[1] = Side::SELL; // '2'
    return arr;
}();

// NOTE: fix values start at 1!
inline constexpr std::array<OrderType, 2> ORD_TYPE_MAP = [] {
    std::array<OrderType, 2> arr;
    arr[0] = OrderType::MARKET;
    arr[1] = OrderType::LIMIT;
    return arr;
}();

// NOTE: fix values DO NOT start at 1!
inline constexpr std::array<TIF, 5> TIF_MAP = [] {
    std::array<TIF, 5> arr; 
    arr[0] = TIF::DAY;
    // note that orders are valid at this point
    // since they have already been validated
    // by previous arrays
    arr[1] = TIF::DAY; // actually GTC
    arr[2] = TIF::DAY; // actually at the open 
    arr[3] = TIF::IOC;
    arr[4] = TIF::FOK;
    return arr;
}();

/** @brief looks up in valid side dispatch table if valid side provided */
inline static uint8_t validate_side(char fix_value) {
    return VALID_SIDES[static_cast<size_t>(fix_value - '0' - 1)] == 1;
}

/** @brief looks up in valid order type dispatch table if valid order type was provided */
inline static uint8_t validate_order_type(char fix_value) {
    return VALID_ORDER_TYPES[static_cast<size_t>(fix_value - '0' - 1)] == 1;
}

/** @brief looks up in the valid tif dispatch table if valid tif provided */
inline static uint8_t validate_tif(char fix_value) {
    return VALID_TIF[static_cast<size_t>(fix_value - '0')] == 1;
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
    o.side = SIDE_MAP[static_cast<size_t>(side - '0' - 1)];
    o.order_type = ORD_TYPE_MAP[static_cast<size_t>(side - '0' - 1)];
    o.tif = TIF_MAP[static_cast<size_t>(side - '0')];
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