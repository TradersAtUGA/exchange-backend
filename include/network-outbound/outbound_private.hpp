/**
 * @brief outbound server routing endpoints via TCP for 
 * client specific information 
 */
#pragma once 
#include <crow.h>
#include <cstdint>

#include "shared/order.hpp"
#include "shared/ring_buffer.hpp"

namespace exchange::network_outbound {

/**
 * @brief allows the user to get status on a order 
 */
void get_order_status(crow::SimpleApp& app);


 /**
  * @brief start the outbound private server
  */
 void start_outbound_server(crow::SimpleApp& app, exchange::RingBuffer<exchange::Order, 1024> buffer);

} // network_outbound