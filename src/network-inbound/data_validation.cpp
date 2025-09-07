/**
 * @file data_validation.cpp
 * @author Vikas Katari
 * @date 2025-09-01
 * @brief Validation of inbound user order requests
 */

#include <cstdint>
#include "crow.h"
#include "../../include/network-inbound/data_validation.hpp"

namespace exchange::network_inbound {
    
    uint8_t validate_order_send_json(const crow::json::rvalue& json) {
        int64_t quantity = static_cast<int64_t>(json["quantity"].i());
        int64_t price = static_cast<int64_t>(json["price"].i());
        
        // Branchless validation
        return (quantity > 0) * (price > 0);
    }

} // network_inbound