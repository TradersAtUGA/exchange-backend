#pragma once
#include "crow.h"

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
void start_inbound_server();