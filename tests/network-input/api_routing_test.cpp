/**
 * @brief test file to test input network server endpoint handling
 */

#include <gtest/gtest.h>   // Google Test
#include "crow.h"
#include "../../include/network-input/api_routing.cpp"
#define CROW_ENABLE_TESTING  // enable testing helpers


TEST(CrowTest, AddRoute) {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/add")([](const crow::request& req){
        int a = std::stoi(req.url_params.get("a"));
        int b = std::stoi(req.url_params.get("b"));
        return std::to_string(a + b);
    });

    crow::request req;
    req.url = "/order-send";

    auto res = app.handle(req);  // simulate a request

    EXPECT_EQ(res.body, "ok");
}
