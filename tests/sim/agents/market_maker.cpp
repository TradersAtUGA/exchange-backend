#include "agents/market_maker.hpp"
#include "helpers/order_factory.hpp"
#include <limits>

namespace sim {

MarketMaker::MarketMaker(uint64_t client_id, MarketMakerConfig config)
    : Agent(client_id), config_(config) {}

std::vector<Message<exchange::Order>> MarketMaker::act(
    const exchange::OrderBook& book,
    uint64_t tick
) {
    if (tick % config_.requote_freq != 0) return {};

    // Compute mid price
    uint64_t best_bid = book.best_bid();
    uint64_t best_ask = book.best_ask();

    uint64_t mid = config_.default_mid;
    if (best_bid > 0 && best_ask < std::numeric_limits<uint64_t>::max()) {
        mid = (best_bid + best_ask) / 2;
    }

    uint64_t bid_price = (mid > config_.spread) ? mid - config_.spread : 1;
    uint64_t ask_price = mid + config_.spread;

    std::vector<Message<exchange::Order>> orders;

    // Post new bid
    last_bid_oid_ = next_oid();
    orders.push_back(sim::make_limit(
        Side::BUY, bid_price, config_.quote_size,
        last_bid_oid_, cid_
    ));

    // Post new ask
    last_ask_oid_ = next_oid();
    orders.push_back(sim::make_limit(
        Side::SELL, ask_price, config_.quote_size,
        last_ask_oid_, cid_
    ));

    return orders;
}

std::vector<Message<exchange::Order>> MarketMaker::cancels(
    const exchange::OrderBook& book,
    uint64_t tick
) {
    (void)book;
    if (tick % config_.requote_freq != 0) return {};
    if (tick == 0) return {};

    std::vector<Message<exchange::Order>> cancel_msgs;

    // Cancel previous quotes before posting new ones
    if (last_bid_oid_ != 0) {
        cancel_msgs.push_back(sim::make_cancel(last_bid_oid_));
    }
    if (last_ask_oid_ != 0) {
        cancel_msgs.push_back(sim::make_cancel(last_ask_oid_));
    }

    return cancel_msgs;
}

} // namespace sim
