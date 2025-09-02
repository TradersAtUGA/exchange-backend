#pragma once 
#include<cstdint>
#include "crow.h"


namespace network_input {

    
    /**
     * @brief validates a JSON coming from the /order-send endpoint
     * 
     * @param json the JSON payload from the client side
     */
    uint8_t validate_order_send_json(const crow::json::rvalue& json);

}
