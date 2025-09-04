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
#include "../../include/network-input/data_validation.hpp"
#include "../../include/network-input/data_conversion.hpp"

/*
1. Set up API endpoints using crow
2. Create handlers to receive JSONs and then validate them 
3. Once validated they should be converted to a order struct
4. Return the order struct since this will be handed to the MPMC LFQ
*/

namespace network_input {

    void register_order_send_route(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/order-send").methods("POST"_method)([](const crow::request& req) {
            
            crow::json::rvalue x = crow::json::load(req.body);
            if (!x) return crow::response(400, "malformed input");

            if (!network_input::validate_order_send_json(x)) return crow::response(400, "invalid argument(s)");

            exchange::Order user_order = network_input::json_to_exchange_order(x);

            // put onto deque, which should be passed by ref here inside the lambda capture

            return crow::response(200, "ok");
        });
    }

    void register_order_modify_route(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/order-modify/<uint>").methods("POST"_method)([](uint64_t id) {
            return "temp";
        });
    }

    void register_order_cancel_route(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/order-cancel/<uint>").methods("POST"_method)([](uint64_t id) {
            return "temp";
        });
    }

    void register_order_get_route(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/order-get/<uint>").methods("GET"_method)([](uint64_t id) {
            return "temp";
        });
    }

    // deque should be passed here by ref as well so the first 
    // route can place orders onto it 
    
    void start_input_server(crow::SimpleApp& app) {
        register_order_send_route(app);
        // add other routes here before startring the server 
        app.port(network_input::PORT_NUMBER).concurrency(network_input::THREAD_POOL_THREAD_COUNT).run();
    }

} // network_input