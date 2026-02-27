#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"
#include "helpers/perf_timer.hpp"
#include <iostream>

using namespace exchange;

TEST(Stress, ThroughputOneMillionOrders) {
    MatchingEngine me("TEST");
    sim::PerfTimer timer;
    constexpr uint64_t N = 1'000'000;

    // Pre-generate alternating buy/sell limit orders at same price
    // This creates maximum matching (every other order matches)
    std::vector<Message<Order>> orders;
    orders.reserve(N);
    for (uint64_t i = 0; i < N; ++i) {
        Side side = (i % 2 == 0) ? Side::BUY : Side::SELL;
        orders.push_back(sim::make_limit(side, 100, 1, /*oid=*/i + 1, /*coid=*/i + 1));
    }

    // Time the processing
    timer.start();
    for (auto& order : orders) {
        me.process(order);
    }
    uint64_t total_ns = timer.stop_ns();

    double total_ms = static_cast<double>(total_ns) / 1'000'000.0;
    double ops_per_sec = static_cast<double>(N) / (static_cast<double>(total_ns) / 1e9);

    std::cout << "\n=== Throughput Test Results ===" << std::endl;
    std::cout << "Orders processed: " << N << std::endl;
    std::cout << "Total time: " << total_ms << " ms" << std::endl;
    std::cout << "Throughput: " << ops_per_sec << " orders/sec" << std::endl;

    // Baseline sanity check: at least 100K orders/sec
    EXPECT_GT(ops_per_sec, 100'000.0) << "Throughput below 100K orders/sec";
}

TEST(Stress, PerOrderLatency) {
    MatchingEngine me("TEST");
    sim::PerfTimer timer;
    constexpr uint64_t N = 10'000;

    // Seed some resting orders
    for (uint64_t i = 0; i < 100; ++i) {
        auto sell = sim::make_limit(Side::SELL, 100 + i, 10, /*oid=*/i + 1);
        me.process(sell);
    }

    // Measure per-order latency for crossing orders
    for (uint64_t i = 0; i < N; ++i) {
        auto buy = sim::make_limit(Side::BUY, 200, 1, /*oid=*/1000 + i);
        timer.start();
        me.process(buy);
        timer.stop_ns();

        // Replenish asks
        auto sell = sim::make_limit(Side::SELL, 100 + (i % 100), 1, /*oid=*/2000 + i);
        me.process(sell);
    }

    std::cout << "\n=== Per-Order Latency ===" << std::endl;
    std::cout << "Samples: " << timer.count() << std::endl;
    std::cout << "Avg: " << timer.avg_ns() << " ns" << std::endl;
    std::cout << "P50: " << timer.p50_ns() << " ns" << std::endl;
    std::cout << "P99: " << timer.p99_ns() << " ns" << std::endl;
}
