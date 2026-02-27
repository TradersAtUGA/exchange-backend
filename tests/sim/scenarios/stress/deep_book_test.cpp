#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"
#include "helpers/perf_timer.hpp"
#include "assertions/orderbook_invariants.hpp"
#include <iostream>

using namespace exchange;

TEST(Stress, DeepBookBuild) {
    MatchingEngine me("TEST");
    sim::PerfTimer timer;

    constexpr uint64_t NUM_LEVELS = 10'000;
    constexpr uint64_t ORDERS_PER_LEVEL = 10;
    constexpr uint64_t TOTAL = NUM_LEVELS * ORDERS_PER_LEVEL;

    // Build deep ask book: price levels 100 to 10099, 10 orders each
    timer.start();
    uint64_t oid = 1;
    for (uint64_t p = 100; p < 100 + NUM_LEVELS; ++p) {
        for (uint64_t i = 0; i < ORDERS_PER_LEVEL; ++i) {
            auto sell = sim::make_limit(Side::SELL, p, 1, oid++);
            me.process(sell);
        }
    }
    uint64_t build_ns = timer.stop_ns();

    std::cout << "\n=== Deep Book Build ===" << std::endl;
    std::cout << "Total orders: " << TOTAL << std::endl;
    std::cout << "Build time: " << static_cast<double>(build_ns) / 1e6 << " ms" << std::endl;

    // Verify best ask
    EXPECT_EQ(me.orderbook().best_ask(), 100);

    // Sweep through multiple levels
    timer.start();
    auto buy = sim::make_limit(Side::BUY, 200, 500, oid++);
    me.process(buy);
    uint64_t sweep_ns = timer.stop_ns();

    std::cout << "Sweep 500 shares time: " << static_cast<double>(sweep_ns) / 1e3 << " us" << std::endl;

    // 500 shares across 50 levels (10 per level) should be consumed
    EXPECT_EQ(me.volume(), 500);

    // Check invariants
    std::string inv = sim::check_orderbook_invariants(me.orderbook());
    EXPECT_TRUE(inv.empty()) << "Invariant violation: " << inv;
}
