#pragma once

#include "helpers/order_factory.hpp"
#include <random>
#include <vector>
#include <cstdint>

namespace sim {

struct GeneratorConfig {
    double buy_ratio = 0.5;           // probability of BUY side
    double limit_ratio = 0.9;         // probability of LIMIT vs MARKET
    double day_ratio = 0.8;           // among non-MARKET: DAY probability
    double ioc_ratio = 0.1;           // IOC probability (rest is FOK)
    uint64_t center_price = 100;      // center of price distribution
    uint64_t price_stddev = 5;        // stddev for price normal distribution
    uint64_t min_qty = 1;
    uint64_t max_qty = 100;
    uint64_t seed = 42;
};

class OrderGenerator {
public:
    explicit OrderGenerator(GeneratorConfig config = {});

    // Generate a single random order
    Message<exchange::Order> next_order();

    // Generate a cancel for a previously generated order (if any exist)
    // Returns nullopt if no orders to cancel
    std::optional<Message<exchange::Order>> next_cancel();

    // Generate N random orders
    std::vector<Message<exchange::Order>> generate(size_t count);

    uint64_t orders_generated() const { return oid_counter_; }

private:
    GeneratorConfig config_;
    std::mt19937_64 rng_;
    uint64_t oid_counter_ = 1;
    std::vector<uint64_t> live_oids_;

    std::uniform_real_distribution<double> uniform_;
    std::normal_distribution<double> price_dist_;
    std::uniform_int_distribution<uint64_t> qty_dist_;
};

} // namespace sim
