/**
 * @file api_routing.cpp
 * @author Vikas Katari 
 * @date 2025-09-01
 * @brief setup crow App for API routing 
 */
#include <iostream>
#include <cstdint>
#include "crow.h"
#include "../../include/order.hpp"
#include "../../include/network-input/api_routing.hpp"

/*
1. Set up API endpoints using crow
2. Create handlers to receive JSONs and then validate them 
3. Once validated they should be converted to a order struct
4. Return the order struct since this will be handed to the MPMC LFQ


API ROUTES
/order-send

This is where orders will be sent to 


/order-modify/<id>
/order-cancel/<id>
/order-get/<id>
*/

namespace network_inbound {
void register_order_send_route(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/order-send").methods("POST"_method)([](const crow::request& req) {
        return "temp";

        /*
        First validate the inbound JSON order

        if its good return 200 to the user

        put the order onto the deque somehow

        else return 400 for a bad order and simply return 
        */
    });
}

void register_order_modify_route(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/order-modify/<uint>")([](uint64_t id) {
        return "temp";
    });
}




void start_input_server(crow::SimpleApp& app) {
    app.port(PORT_NUMBER).concurrency(THREAD_POOL_THREAD_COUNT).run();
}


void add(int a, int b) {
    std::cout << a + b << "\n";
}
}