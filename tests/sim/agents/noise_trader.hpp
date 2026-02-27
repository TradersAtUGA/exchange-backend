#pragma once

#include "agents/agent.hpp"
#include <random>

namespace sim {

struct NoiseTraderConfig {
    double activity_rate = 0.5;    // probability of sending an order each tick
    uint64_t center_price = 100;
    uint64_t price_stddev = 5;
    uint64_t min_qty = 1;
    uint64_t max_qty = 20;
    double buy_ratio = 0.5;
    uint64_t seed = 42;
};

class NoiseTrader : public Agent {
public:
    NoiseTrader(uint64_t client_id, NoiseTraderConfig config = {});

    std::vector<Message<exchange::Order>> act(
        const exchange::OrderBook& book,
        uint64_t tick
    ) override;

private:
    NoiseTraderConfig config_;
    std::mt19937_64 rng_;
    std::uniform_real_distribution<double> uniform_;
    std::normal_distribution<double> price_dist_;
    std::uniform_int_distribution<uint64_t> qty_dist_;
};

} // namespace sim
