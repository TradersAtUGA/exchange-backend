#pragma once

#include "agents/agent.hpp"
#include <random>

namespace sim {

struct AggressiveTraderConfig {
    double activity_rate = 0.3;
    uint64_t min_qty = 1;
    uint64_t max_qty = 50;
    double market_ratio = 0.3;   // probability of sending market vs crossing limit
    double buy_ratio = 0.5;
    uint64_t seed = 42;
};

class AggressiveTrader : public Agent {
public:
    AggressiveTrader(uint64_t client_id, AggressiveTraderConfig config = {});

    std::vector<Message<exchange::Order>> act(
        const exchange::OrderBook& book,
        uint64_t tick
    ) override;

private:
    AggressiveTraderConfig config_;
    std::mt19937_64 rng_;
    std::uniform_real_distribution<double> uniform_;
    std::uniform_int_distribution<uint64_t> qty_dist_;
};

} // namespace sim
