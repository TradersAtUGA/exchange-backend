#pragma once 
#include<cstdint>
#include "crow.h"

/**
 * @brief validates a order json from the inbound network 
 * 
 * @param json the json from the client side for a order send
 */
uint8_t validate_order_send(crow::json::rvalue& json);