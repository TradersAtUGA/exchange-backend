#include "agents/aggressive_trader.hpp"
#include "helpers/order_factory.hpp"
#include <limits>

namespace sim {

AggressiveTrader::AggressiveTrader(uint64_t client_id, AggressiveTraderConfig config)
    : Agent(client_id),
      config_(config),
      rng_(config.seed + client_id),
      uniform_(0.0, 1.0),
      qty_dist_(config.min_qty, config.max_qty)
{}

std::vector<Message<exchange::Order>> AggressiveTrader::act(
    const exchange::OrderBook& book,
    uint64_t tick
) {
    (void)tick;

    if (uniform_(rng_) > config_.activity_rate) return {};

    Side side = (uniform_(rng_) < config_.buy_ratio) ? Side::BUY : Side::SELL;
    uint64_t qty = qty_dist_(rng_);

    bool use_market = (uniform_(rng_) < config_.market_ratio);

    if (use_market) {
        return {sim::make_market(side, qty, next_oid(), cid_)};
    }

    // Crossing limit: price at or through the best opposing quote
    uint64_t price;
    if (side == Side::BUY) {
        price = book.best_ask();
        // Don't use sentinel price
        if (price == std::numeric_limits<uint64_t>::max()) price = 100;
    } else {
        price = book.best_bid();
        if (price == 0) price = 100;
    }

    return {sim::make_limit(side, price, qty, next_oid(), cid_)};
}

} // namespace sim
