#include <gtest/gtest.h>
#include "agents/market_maker.hpp"
#include "agents/noise_trader.hpp"
#include "agents/aggressive_trader.hpp"
#include "assertions/orderbook_invariants.hpp"
#include "helpers/perf_timer.hpp"
#include <iostream>
#include <memory>

using namespace exchange;

// Scenario: Full Simulation10 KTicks.
TEST(MultiAgent, FullSimulation10KTicks) {
    MatchingEngine me("TEST");

    // Create agents
    std::vector<std::unique_ptr<sim::Agent>> agents;

    // 2 Market Makers
    sim::MarketMakerConfig mm_cfg;
    mm_cfg.spread = 2;
    mm_cfg.quote_size = 20;
    mm_cfg.requote_freq = 3;
    mm_cfg.default_mid = 100;
    agents.push_back(std::make_unique<sim::MarketMaker>(1, mm_cfg));
    agents.push_back(std::make_unique<sim::MarketMaker>(2, mm_cfg));

    // 5 Noise Traders
    for (uint64_t i = 3; i <= 7; ++i) {
        sim::NoiseTraderConfig nt_cfg;
        nt_cfg.activity_rate = 0.4;
        nt_cfg.center_price = 100;
        nt_cfg.price_stddev = 5;
        nt_cfg.seed = i * 100;
        agents.push_back(std::make_unique<sim::NoiseTrader>(i, nt_cfg));
    }

    // 2 Aggressive Traders
    for (uint64_t i = 8; i <= 9; ++i) {
        sim::AggressiveTraderConfig at_cfg;
        at_cfg.activity_rate = 0.2;
        at_cfg.market_ratio = 0.3;
        at_cfg.seed = i * 200;
        agents.push_back(std::make_unique<sim::AggressiveTrader>(i, at_cfg));
    }

    constexpr uint64_t NUM_TICKS = 10'000;
    constexpr uint64_t INVARIANT_CHECK_INTERVAL = 100;

    sim::PerfTimer timer;
    uint64_t total_orders = 0;

    timer.start();

    for (uint64_t tick = 0; tick < NUM_TICKS; ++tick) {
        for (auto& agent : agents) {
            // Process cancels first
            auto cancel_msgs = agent->cancels(me.orderbook(), tick);
            for (auto& c : cancel_msgs) {
                me.cancel(c);
            }

            // Then new orders
            auto orders = agent->act(me.orderbook(), tick);
            for (auto& order : orders) {
                me.process(order);
                ++total_orders;
            }
        }

        if (tick % INVARIANT_CHECK_INTERVAL == 0) {
            std::string inv = sim::check_orderbook_invariants(me.orderbook());
            EXPECT_TRUE(inv.empty())
                << "Invariant violation at tick " << tick << ": " << inv;
        }
    }

    uint64_t total_ns = timer.stop_ns();

    // Final invariant check
    std::string inv = sim::check_orderbook_invariants(me.orderbook());
    EXPECT_TRUE(inv.empty()) << "Final invariant violation: " << inv;

    double total_ms = static_cast<double>(total_ns) / 1e6;
    double ops_per_sec = static_cast<double>(total_orders) /
                         (static_cast<double>(total_ns) / 1e9);

    std::cout << "\n=== Multi-Agent Simulation Results ===" << std::endl;
    std::cout << "Ticks: " << NUM_TICKS << std::endl;
    std::cout << "Total orders processed: " << total_orders << std::endl;
    std::cout << "Total volume: " << me.volume() << std::endl;
    std::cout << "Total time: " << total_ms << " ms" << std::endl;
    std::cout << "Throughput: " << ops_per_sec << " orders/sec" << std::endl;
    std::cout << "Best bid: " << me.bid() << std::endl;
    std::cout << "Best ask: " << me.ask() << std::endl;
}
