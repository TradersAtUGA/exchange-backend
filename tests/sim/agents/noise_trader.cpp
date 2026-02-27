#include "agents/noise_trader.hpp"
#include "helpers/order_factory.hpp"
#include <algorithm>

namespace sim {

NoiseTrader::NoiseTrader(uint64_t client_id, NoiseTraderConfig config)
    : Agent(client_id),
      config_(config),
      rng_(config.seed + client_id),
      uniform_(0.0, 1.0),
      price_dist_(static_cast<double>(config.center_price),
                  static_cast<double>(config.price_stddev)),
      qty_dist_(config.min_qty, config.max_qty)
{}

std::vector<Message<exchange::Order>> NoiseTrader::act(
    const exchange::OrderBook& book,
    uint64_t tick
) {
    (void)book;
    (void)tick;

    if (uniform_(rng_) > config_.activity_rate) return {};

    Side side = (uniform_(rng_) < config_.buy_ratio) ? Side::BUY : Side::SELL;

    double raw_price = price_dist_(rng_);
    uint64_t price = static_cast<uint64_t>(std::max(1.0, raw_price));
    uint64_t qty = qty_dist_(rng_);

    // Weighted TIF: 80% DAY, 10% IOC, 10% FOK
    double tif_roll = uniform_(rng_);
    TIF tif = TIF::DAY;
    if (tif_roll > 0.9)
        tif = TIF::FOK;
    else if (tif_roll > 0.8)
        tif = TIF::IOC;

    exchange::Order o = exchange::Order::create_new_order(
        side, OrderType::LIMIT, tif,
        const_cast<char*>("TEST"),
        price, qty, next_oid(), cid_
    );

    Message<exchange::Order> msg;
    msg.payload = o;
    return {msg};
}

} // namespace sim
