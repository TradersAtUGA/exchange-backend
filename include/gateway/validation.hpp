/** @brief validation logic and internal order generation for incoming orders */
#pragma once 

#include <cstdint>
#include <string>
#include <array>
#include <bitset>

#include "shared/order.hpp"
#include "shared/enums.hpp"
#include "gateway/gateway.hpp"
#include "shared/utilities.hpp"

namespace exchange {

consteval std::array<uint8_t, 128> valid_sides() { 
    std::array<uint8_t, 128> arr; 
    arr['1'] = 1; 
    arr['2'] = 1; 
    return arr; 
}

consteval std::array<uint8_t, 128> valid_ord_types() { 
    std::array<uint8_t, 128> arr; 
    arr['1'] = 1; 
    arr['2'] = 1; 
    return arr; 
}

consteval std::array<uint8_t, 128> valid_tif() { 
    std::array<uint8_t, 128> arr; 
    arr['0'] = 1; // DAY 
    arr['3'] = 1; // IOC 
    arr['4'] = 1; // FOK
    return arr; 
}

constexpr std::array<uint8_t, 128> VALID_SIDES = valid_sides();
constexpr std::array<uint8_t, 128> VALID_ORDER_TYPES = valid_ord_types(); 
constexpr std::array<uint8_t, 128> VALID_TIF = valid_tif(); 

// MAPPING ARRAYS 

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

/** @brief creates a new order given that its valid */
inline exchange::Order generate_order(
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
    o.order_type = ORD_TYPE_MAP[static_cast<size_t>(order_type- '0' - 1)];
    o.tif = TIF_MAP[static_cast<size_t>(tif - '0')];
    o.price = exchange::double_to_uint64_t(price);
    o.qty = static_cast<uint64_t>(qty); // avoid bit casts for qty  

    // internally filled fields 
    o.status = Status::NEW; // order is alive/valid
    o.oid = 0; // TODO: SEQUENCER MUST ASSIGN HERE

    return o;
}

/** @brief validates fix passed values */
inline uint8_t validate_fix_values(
    char side, 
    char order_type, 
    char tif, 
    double price, 
    double qty
) {
    return 
    (VALID_SIDES[side] == 1)
    && (VALID_ORDER_TYPES[order_type] == 1)
    && (VALID_TIF[tif] == 1)
    && static_cast<uint8_t>(price > 0)
    && static_cast<uint8_t>(qty > 0);
}

} // namespace exchange         