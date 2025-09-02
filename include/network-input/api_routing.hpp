#pragma once
#include "crow.h"
#include <cstdint>


// port number to run the inbound server on
inline constexpr uint32_t PORT_NUMBER = 10000;

// number of threads in the thread pool
inline constexpr uint8_t THREAD_POOL_THREAD_COUNT = 4;

namespace network_input {

    /**
     * @brief sets up the order send route for the inbound network API
     * for users to create orders
     * 
     * @param app the main crow::SimpleApp 
     */
    void register_order_send_route(crow::SimpleApp& app);

    /**
     * @brief sets up the endpoint to modify orders that have been 
     * placed given an order id
     * 
     * @param app the main crow::SimpleApp
     */
    void register_order_modify_route(crow::SimpleApp& app);

    /**
     * @brief sets up the endpoint to cancel orders that have been 
     * placed given an order id
     * 
     * @param app the main crow::SimpleApp
     */
    void register_order_cancel_route(crow::SimpleApp& app);

    /**
     * @brief sets up the endpoint to get orders that have been 
     * placed given an order id
     * 
     * @param app the main crow::SimpleApp
     */
    void register_order_get_route(crow::SimpleApp& app);

    /**
     * @brief starts the input network server for all input requests
     * 
     * @param app the main crow::SimpleApp
     */
    void start_input_server(crow::SimpleApp& app);

} // network_input

