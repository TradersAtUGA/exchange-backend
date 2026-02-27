#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"

using namespace exchange;

TEST(PriceTimePriority, FifoWithinSamePriceLevel) {
    MatchingEngine me("TEST");

    // Seed 100 sells at $10, 1 share each, oid 1..100
    for (uint64_t i = 1; i <= 100; ++i) {
        auto sell = sim::make_limit(Side::SELL, 10, 1, /*oid=*/i, /*coid=*/i);
        me.process(sell);
    }

    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 100);
    EXPECT_EQ(me.orderbook().asks().at(10).order_count(), 100);

    // Send a buy at $10 for 50 shares — should match the first 50 sells (FIFO)
    auto buy = sim::make_limit(Side::BUY, 10, 50, /*oid=*/200, /*coid=*/999);
    me.process(buy);

    // 50 sells should remain at price 10
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 50);
    EXPECT_EQ(me.orderbook().asks().at(10).order_count(), 50);

    // The front of the queue should now be oid=51 (first 50 were consumed)
    EXPECT_EQ(me.orderbook().asks().at(10).front().payload.oid, 51);

    // Volume should reflect 50 shares traded
    EXPECT_EQ(me.volume(), 50);
}

TEST(PriceTimePriority, BetterPriceMatchesFirst) {
    MatchingEngine me("TEST");

    // Place sells at different prices
    auto sell_12 = sim::make_limit(Side::SELL, 12, 10, /*oid=*/1);
    auto sell_10 = sim::make_limit(Side::SELL, 10, 10, /*oid=*/2);
    auto sell_11 = sim::make_limit(Side::SELL, 11, 10, /*oid=*/3);
    me.process(sell_12);
    me.process(sell_10);
    me.process(sell_11);

    // Buy 25 shares at $12 — should fill at 10, then 11, then partial at 12
    auto buy = sim::make_limit(Side::BUY, 12, 25, /*oid=*/100);
    me.process(buy);

    // Price 10: fully consumed (10 shares)
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 0);
    // Price 11: fully consumed (10 shares)
    EXPECT_EQ(me.orderbook().asks().at(11).total_shares(), 0);
    // Price 12: 5 shares remaining (10 - 5 = 5)
    EXPECT_EQ(me.orderbook().asks().at(12).total_shares(), 5);

    EXPECT_EQ(me.volume(), 25);
}

TEST(PriceTimePriority, BidSidePriceTimePriority) {
    MatchingEngine me("TEST");

    // Place bids at different prices
    auto bid_10 = sim::make_limit(Side::BUY, 10, 10, /*oid=*/1);
    auto bid_12 = sim::make_limit(Side::BUY, 12, 10, /*oid=*/2);
    auto bid_11 = sim::make_limit(Side::BUY, 11, 10, /*oid=*/3);
    me.process(bid_10);
    me.process(bid_12);
    me.process(bid_11);

    // Sell 25 shares at $10 — should match best bid first (12, then 11, then 10)
    auto sell = sim::make_limit(Side::SELL, 10, 25, /*oid=*/100);
    me.process(sell);

    // Price 12: fully consumed
    EXPECT_EQ(me.orderbook().bids().at(12).total_shares(), 0);
    // Price 11: fully consumed
    EXPECT_EQ(me.orderbook().bids().at(11).total_shares(), 0);
    // Price 10: 5 shares remaining
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 5);

    EXPECT_EQ(me.volume(), 25);
}
