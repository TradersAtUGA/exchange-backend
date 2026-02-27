#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"

using namespace exchange;

TEST(DayLimitResting, BuyBelowAskRestsOnBook) {
    MatchingEngine me("TEST");

    // Place a sell at $10
    auto sell = sim::make_limit(Side::SELL, 10, 50, /*oid=*/1);
    me.process(sell);

    // Place a buy at $9 — does not cross, should rest on bids
    auto buy = sim::make_limit(Side::BUY, 9, 30, /*oid=*/2);
    me.process(buy);

    // No trade should occur
    EXPECT_EQ(me.volume(), 0);

    // Sell still on book
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 50);

    // Buy rests on bids
    EXPECT_EQ(me.orderbook().bids().at(9).total_shares(), 30);
    EXPECT_EQ(me.orderbook().bids().at(9).order_count(), 1);
}

TEST(DayLimitResting, SellAboveBidRestsOnBook) {
    MatchingEngine me("TEST");

    // Place a buy at $10
    auto buy = sim::make_limit(Side::BUY, 10, 50, /*oid=*/1);
    me.process(buy);

    // Place a sell at $11 — does not cross, should rest on asks
    auto sell = sim::make_limit(Side::SELL, 11, 30, /*oid=*/2);
    me.process(sell);

    EXPECT_EQ(me.volume(), 0);
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 50);
    EXPECT_EQ(me.orderbook().asks().at(11).total_shares(), 30);
}

TEST(DayLimitResting, RestingOrderThenCrossingMatch) {
    MatchingEngine me("TEST");

    // Buy rests at $9
    auto buy = sim::make_limit(Side::BUY, 9, 30, /*oid=*/1);
    me.process(buy);
    EXPECT_EQ(me.volume(), 0);

    // Now sell at $9 crosses and matches
    auto sell = sim::make_limit(Side::SELL, 9, 30, /*oid=*/2);
    me.process(sell);

    EXPECT_EQ(me.volume(), 30);
    EXPECT_EQ(me.orderbook().bids().at(9).total_shares(), 0);
}
