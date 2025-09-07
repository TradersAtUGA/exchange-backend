#include <gtest/gtest.h>
#include <crow.h>
#include "network-inbound/data_conversion.hpp"
#include "../../include/order.hpp"

TEST(network_inbound_test, good_json_conversion_test) {

    std::string json_str = R"({
        "client_id": 1,
        "order_id": 42,
        "side": "buy",
        "type": "market",
        "ticker": "XYZ",
        "price": 10,
        "quantity": 100,
        "timestamp": 10,
        "remaining": 10
    })";

    crow::json::rvalue good = crow::json::load(json_str);

    
    exchange::Order order = exchange::network_inbound::json_to_exchange_order(good);


}