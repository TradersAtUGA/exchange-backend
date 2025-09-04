#pragma once
#include "crow.h"
#include "moodycamel/concurrentqueue.h"
#include "../order.hpp"
#include <cstdint>

namespace network_input {

    // port number to run the inbound server on
    constexpr uint32_t PORT_NUMBER = 10000;

    // number of threads in the thread pool
    constexpr uint8_t THREAD_POOL_THREAD_COUNT = 4;

    /**
     * @brief sets up the order send route for the inbound network API
     * for users to create orders
     * 
     * @param app the main crow::SimpleApp 
     * @param q the queue that orders will be placed onto
     */
    void register_order_send_route(crow::SimpleApp& app, moodycamel::ConcurrentQueue<exchange::Order>& q);

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
     * @param q the queue to place inbound orders onto
     */
    void start_input_server(crow::SimpleApp& app, moodycamel::ConcurrentQueue<exchange::Order>& q);

} // network_input

