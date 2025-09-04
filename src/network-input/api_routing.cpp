#include <iostream>
#include <cstdint>
#include "crow.h"
#include "moodycamel/concurrentqueue.h"
#include "../../include/order.hpp"
#include "../../include/network-input/api_routing.hpp"
#include "../../include/network-input/data_validation.hpp"
#include "../../include/network-input/data_conversion.hpp"

namespace network_input {

    void register_order_send_route(crow::SimpleApp& app, moodycamel::ConcurrentQueue<exchange::Order>& q) {
        CROW_ROUTE(app, "/order-send").methods("POST"_method)([&q](const crow::request& req) {
            crow::json::rvalue x = crow::json::load(req.body);

            if (!x) return crow::response(400, "malformed input");

            if (!network_input::validate_order_send_json(x)) return crow::response(400, "invalid argument(s)");

            exchange::Order user_order = network_input::json_to_exchange_order(x);

            q.enqueue(user_order);
            std::cout << q.size_approx() << "\n";

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

    void start_input_server(crow::SimpleApp& app, moodycamel::ConcurrentQueue<exchange::Order>& q) {
        register_order_send_route(app, q);
        // add other routes here before startring the server 
        app.port(network_input::PORT_NUMBER).concurrency(network_input::THREAD_POOL_THREAD_COUNT).run();
    }

} // network_input