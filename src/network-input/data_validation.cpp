/**
 * @file data_validation.cpp
 * @author Vikas Katari
 * @date 2025-09-01
 * @brief Validation of inbound user order requests
 */

#include <cstdint>
#include "crow.h"
#include "../../include/order.hpp"
#include "../../include/network-input/data_validation.hpp"

namespace network_input {
    
    uint8_t validate_order_send_json(const crow::json::rvalue& json) {

        int64_t quantity = static_cast<int64_t>(json["quantity"].i());
        if (quantity <= 0) return 0;

        int64_t price = static_cast<int64_t>(json["price"].i());
        if (price <= 0) return 0;

        return 1;
    }

} // network_input