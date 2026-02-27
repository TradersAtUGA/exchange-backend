#include <gtest/gtest.h>
#include "helpers/order_generator.hpp"
#include "assertions/orderbook_invariants.hpp"

using namespace exchange;

// Scenario: Single Share Orders.
TEST(EdgeCase, SingleShareOrders) {
    MatchingEngine me("TEST");

    sim::GeneratorConfig cfg;
    cfg.seed = 777;
    cfg.min_qty = 1;
    cfg.max_qty = 1;  // all single-share orders
    cfg.center_price = 50;
    cfg.price_stddev = 5;
    cfg.limit_ratio = 1.0;
    cfg.day_ratio = 1.0;

    sim::OrderGenerator gen(cfg);

    constexpr size_t N = 1000;

    for (size_t i = 0; i < N; ++i) {
        auto order = gen.next_order();
        me.process(order);

        if (i % 100 == 0) {
            std::string inv = sim::check_orderbook_invariants(me.orderbook());
            EXPECT_TRUE(inv.empty()) << "Invariant at order " << i << ": " << inv;
        }
    }

    // Final check
    std::string inv = sim::check_orderbook_invariants(me.orderbook());
    EXPECT_TRUE(inv.empty()) << "Final invariant: " << inv;
}
