#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"

using namespace exchange;

TEST(MarketOrder, BuySweepsMultipleAskLevels) {
    MatchingEngine me("TEST");

    // Place asks at different prices
    auto s1 = sim::make_limit(Side::SELL, 10, 10, /*oid=*/1);
    auto s2 = sim::make_limit(Side::SELL, 11, 10, /*oid=*/2);
    auto s3 = sim::make_limit(Side::SELL, 12, 10, /*oid=*/3);
    me.process(s1);
    me.process(s2);
    me.process(s3);

    // Market buy for 25 shares — sweeps 10, 11, partial 12
    auto mkt = sim::make_market(Side::BUY, 25, /*oid=*/100);
    me.process(mkt);

    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 0);
    EXPECT_EQ(me.orderbook().asks().at(11).total_shares(), 0);
    EXPECT_EQ(me.orderbook().asks().at(12).total_shares(), 5);
    EXPECT_EQ(me.volume(), 25);
}

TEST(MarketOrder, SellSweepsMultipleBidLevels) {
    MatchingEngine me("TEST");

    auto b1 = sim::make_limit(Side::BUY, 12, 10, /*oid=*/1);
    auto b2 = sim::make_limit(Side::BUY, 11, 10, /*oid=*/2);
    auto b3 = sim::make_limit(Side::BUY, 10, 10, /*oid=*/3);
    me.process(b1);
    me.process(b2);
    me.process(b3);

    // Market sell for 25 — sweeps best bid first (12, 11, partial 10)
    auto mkt = sim::make_market(Side::SELL, 25, /*oid=*/100);
    me.process(mkt);

    EXPECT_EQ(me.orderbook().bids().at(12).total_shares(), 0);
    EXPECT_EQ(me.orderbook().bids().at(11).total_shares(), 0);
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 5);
    EXPECT_EQ(me.volume(), 25);
}

TEST(MarketOrder, MarketOrderOnEmptyBookDoesNotCrash) {
    MatchingEngine me("TEST");

    // Market buy on empty book — should not crash
    // The only ask is the sentinel at UINT64_MAX which should not match
    auto mkt = sim::make_market(Side::BUY, 50, /*oid=*/1);
    me.process(mkt);

    // Market orders don't rest (they use DAY TIF but with MARKET type,
    // and on_partial_fill_aggressive_limit_ checks ord_type != MARKET)
    EXPECT_EQ(me.volume(), 0);
}
