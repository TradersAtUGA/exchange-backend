#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"
#include "assertions/orderbook_invariants.hpp"
#include <limits>

using namespace exchange;

// Scenario: Minimum Real Price.
TEST(EdgeCase, MinimumRealPrice) {
    MatchingEngine me("TEST");

    // Price=1 is the minimum real price (0 is bid sentinel)
    auto sell = sim::make_limit(Side::SELL, 1, 10, /*oid=*/1);
    me.process(sell);

    EXPECT_EQ(me.orderbook().asks().at(1).total_shares(), 10);

    auto buy = sim::make_limit(Side::BUY, 1, 10, /*oid=*/2);
    me.process(buy);

    EXPECT_EQ(me.volume(), 10);
}

// Scenario: Maximum Real Price.
TEST(EdgeCase, MaximumRealPrice) {
    MatchingEngine me("TEST");

    // UINT64_MAX - 1 is the maximum real price (UINT64_MAX is ask sentinel)
    uint64_t max_price = std::numeric_limits<uint64_t>::max() - 1;

    auto buy = sim::make_limit(Side::BUY, max_price, 10, /*oid=*/1);
    me.process(buy);

    EXPECT_EQ(me.orderbook().bids().at(max_price).total_shares(), 10);

    auto sell = sim::make_limit(Side::SELL, max_price, 10, /*oid=*/2);
    me.process(sell);

    EXPECT_EQ(me.volume(), 10);
}

// Scenario: Price At Ask Sentinel.
TEST(EdgeCase, PriceAtAskSentinel) {
    MatchingEngine me("TEST");

    // Price=UINT64_MAX is the ask sentinel price
    // Adding an order at this price may interact with the sentinel
    uint64_t sentinel_price = std::numeric_limits<uint64_t>::max();

    auto sell = sim::make_limit(Side::SELL, sentinel_price, 10, /*oid=*/1);
    me.process(sell);

    // The order should go into the sentinel price level
    // This documents the behavior — it should not crash
    EXPECT_GE(me.orderbook().asks().at(sentinel_price).total_shares(), 10);
}

// Scenario: Price At Bid Sentinel.
TEST(EdgeCase, PriceAtBidSentinel) {
    MatchingEngine me("TEST");

    // Price=0 is the bid sentinel price
    auto buy = sim::make_limit(Side::BUY, 0, 10, /*oid=*/1);
    me.process(buy);

    // Documents behavior at sentinel price
    EXPECT_GE(me.orderbook().bids().at(0).total_shares(), 10);
}
