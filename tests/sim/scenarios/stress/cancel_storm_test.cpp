#include <gtest/gtest.h>
#include "helpers/order_generator.hpp"
#include "assertions/orderbook_invariants.hpp"
#include <random>

using namespace exchange;

TEST(Stress, CancelStorm) {
    MatchingEngine me("TEST");

    sim::GeneratorConfig cfg;
    cfg.seed = 12345;
    cfg.buy_ratio = 0.5;
    cfg.limit_ratio = 1.0;  // all limit orders (so they rest)
    cfg.day_ratio = 1.0;    // all DAY
    cfg.center_price = 100;
    cfg.price_stddev = 20;
    cfg.min_qty = 1;
    cfg.max_qty = 10;

    sim::OrderGenerator gen(cfg);

    constexpr size_t NUM_ORDERS = 10'000;
    constexpr size_t INVARIANT_CHECK_INTERVAL = 100;

    // Phase 1: Add all orders
    auto orders = gen.generate(NUM_ORDERS);
    for (auto& order : orders) {
        me.process(order);
    }

    // Phase 2: Cancel 50% randomly
    std::mt19937_64 rng(54321);
    size_t cancels_issued = 0;
    for (size_t i = 0; i < NUM_ORDERS / 2; ++i) {
        auto cancel_opt = gen.next_cancel();
        if (!cancel_opt.has_value()) break;

        me.cancel(cancel_opt.value());
        ++cancels_issued;

        if (cancels_issued % INVARIANT_CHECK_INTERVAL == 0) {
            std::string inv = sim::check_orderbook_invariants(me.orderbook());
            EXPECT_TRUE(inv.empty())
                << "Invariant violation after " << cancels_issued
                << " cancels: " << inv;
        }
    }

    // Final invariant check
    std::string inv = sim::check_orderbook_invariants(me.orderbook());
    EXPECT_TRUE(inv.empty()) << "Final invariant violation: " << inv;

    std::cout << "\n=== Cancel Storm ===" << std::endl;
    std::cout << "Orders placed: " << NUM_ORDERS << std::endl;
    std::cout << "Cancels issued: " << cancels_issued << std::endl;
}
