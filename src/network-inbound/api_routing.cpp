#include <iostream>
#include <cstdint>
#include "crow.h"
#include "moodycamel/concurrentqueue.h"
#include <network-inbound/api_routing.hpp>
#include <network-inbound/data_conversion.hpp>
#include <network-inbound/data_validation.hpp>

#include "../../build/config.hpp"

namespace network_inbound {

    void register_order_send_route(crow::SimpleApp& app, moodycamel::ConcurrentQueue<exchange::Order>& q) {
        CROW_ROUTE(app, "/order-send").methods("POST"_method)([&q](const crow::request& req) {
            crow::json::rvalue x = crow::json::load(req.body);

            if (!x) return crow::response(400, "malformed input");

            if (!network_inbound::validate_order_send_json(x)) return crow::response(400, "invalid argument(s)");

            exchange::Order user_order = network_inbound::json_to_exchange_order(x);

            q.enqueue(user_order);
            std::cout << "Queue contains " << q.size_approx() << " items" << "\n";
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
        app.port(config::NETWORK_INBOUND_PORT_NUMBER).concurrency(config::NETWORK_INBOUND_THREAD_COUNT).run();
    }

} // network_inbound