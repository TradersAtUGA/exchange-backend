#pragma once

#include "agents/agent.hpp"
#include <limits>

namespace sim {

struct MarketMakerConfig {
    uint64_t spread = 2;         // half-spread from mid
    uint64_t quote_size = 10;    // shares per quote
    uint64_t requote_freq = 5;   // ticks between requotes
    uint64_t default_mid = 100;  // fallback mid price
};

class MarketMaker : public Agent {
public:
    MarketMaker(uint64_t client_id, MarketMakerConfig config = {});

    std::vector<Message<exchange::Order>> act(
        const exchange::OrderBook& book,
        uint64_t tick
    ) override;

    std::vector<Message<exchange::Order>> cancels(
        const exchange::OrderBook& book,
        uint64_t tick
    ) override;

private:
    MarketMakerConfig config_;
    uint64_t last_bid_oid_ = 0;
    uint64_t last_ask_oid_ = 0;
};

} // namespace sim
