#include <gtest/gtest.h>
#include <crow.h>
#include <cstdint>
#include "network-inbound/data_validation.hpp"


TEST(network_inbound_test, good_json_validation_test) {

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

    uint8_t result = exchange::network_inbound::validate_order_send_json(good);

    EXPECT_EQ(result, 1);

}

TEST(network_inbound_test, bad_json_validation_test) {

    std::string json_str = R"({
        "client_id": 1,
        "order_id": 42,
        "side": "buy",
        "type": "market",
        "ticker": "XYZ",
        "price": -10,
        "quantity": 100,
        "timestamp": 10,
        "remaining": 10
    })";

    crow::json::rvalue good = crow::json::load(json_str);

    uint8_t result = exchange::network_inbound::validate_order_send_json(good);

    EXPECT_EQ(result, 0);

}