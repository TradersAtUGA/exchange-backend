#pragma once
#include "crow.h"
#include <cstdint>

// port number to run the inbound server on
inline constexpr uint32_t PORT_NUMBER = 10000;

// number of threads in the thread pool
inline constexpr uint8_t THREAD_POOL_THREAD_COUNT = 4;

namespace network_inbound {

/**
 * @brief sets up the order send route for the inbound network API
 * for users to create orders
 * 
 * @param app the main crow SimpleApp 
 */
void register_order_send_route(crow::SimpleApp& app);

/**
 * @brief starts the inbound network server for order requests
 * by setting up the apps endpoints and starting the server
 */
void start_input_server(crow::SimpleApp& app);

}

