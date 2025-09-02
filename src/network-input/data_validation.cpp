/**
 * @file data_validation.cpp
 * @author Vikas Katari
 * @date 2025-09-01
 * @brief Validation of inbound user order requests
 */

#include <cstdint>
#include "crow.h"
#include "../../include/order.hpp"


uint8_t validate_order_send(crow::json::rvalue& json) {
    uint64_t quantity = static_cast<uint64_t>(json["quantity"].i());
    if quantity <= 0 return 0;

    uint64_t price = static_cast<uint64_t>(json["price"].i());
    if price <= 0 return 0;

    return 1
}