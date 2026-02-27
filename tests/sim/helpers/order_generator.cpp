#include "helpers/order_generator.hpp"
#include <algorithm>

namespace sim {

OrderGenerator::OrderGenerator(GeneratorConfig config)
    : config_(config),
      rng_(config.seed),
      uniform_(0.0, 1.0),
      price_dist_(static_cast<double>(config.center_price),
                  static_cast<double>(config.price_stddev)),
      qty_dist_(config.min_qty, config.max_qty)
{}

Message<exchange::Order> OrderGenerator::next_order() {
    uint64_t oid = oid_counter_++;

    Side side = (uniform_(rng_) < config_.buy_ratio) ? Side::BUY : Side::SELL;

    bool is_market = (uniform_(rng_) >= config_.limit_ratio);

    TIF tif = TIF::DAY;
    if (!is_market) {
        double tif_roll = uniform_(rng_);
        if (tif_roll < config_.day_ratio)
            tif = TIF::DAY;
        else if (tif_roll < config_.day_ratio + config_.ioc_ratio)
            tif = TIF::IOC;
        else
            tif = TIF::FOK;
    }

    uint64_t qty = qty_dist_(rng_);
    uint64_t price = 0;
    if (!is_market) {
        double raw_price = price_dist_(rng_);
        price = static_cast<uint64_t>(std::max(1.0, raw_price));
    }

    OrderType ot = is_market ? OrderType::MARKET : OrderType::LIMIT;

    exchange::Order o = exchange::Order::create_new_order(
        side, ot, tif,
        const_cast<char*>("TEST"),
        price, qty, oid, oid
    );

    Message<exchange::Order> msg;
    msg.payload = o;

    if (!is_market && tif == TIF::DAY) {
        live_oids_.push_back(oid);
    }

    return msg;
}

std::optional<Message<exchange::Order>> OrderGenerator::next_cancel() {
    if (live_oids_.empty()) return std::nullopt;

    std::uniform_int_distribution<size_t> idx_dist(0, live_oids_.size() - 1);
    size_t idx = idx_dist(rng_);
    uint64_t target_oid = live_oids_[idx];

    // Remove from live list
    live_oids_.erase(live_oids_.begin() + static_cast<ptrdiff_t>(idx));

    return sim::make_cancel(target_oid, oid_counter_++);
}

std::vector<Message<exchange::Order>> OrderGenerator::generate(size_t count) {
    std::vector<Message<exchange::Order>> orders;
    orders.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        orders.push_back(next_order());
    }
    return orders;
}

} // namespace sim
