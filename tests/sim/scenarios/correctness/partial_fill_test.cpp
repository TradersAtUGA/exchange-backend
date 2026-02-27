#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"

using namespace exchange;

TEST(PartialFill, BuyConsumesMultipleSells) {
    MatchingEngine me("TEST");

    // Place sells: 30, 30, 40 shares all at $10
    auto s1 = sim::make_limit(Side::SELL, 10, 30, /*oid=*/1, /*coid=*/10);
    auto s2 = sim::make_limit(Side::SELL, 10, 30, /*oid=*/2, /*coid=*/20);
    auto s3 = sim::make_limit(Side::SELL, 10, 40, /*oid=*/3, /*coid=*/30);
    me.process(s1);
    me.process(s2);
    me.process(s3);

    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 100);

    // Buy 100 shares at $10 — should consume all three sells
    auto buy = sim::make_limit(Side::BUY, 10, 100, /*oid=*/100, /*coid=*/999);
    me.process(buy);

    // All sells consumed
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 0);
    EXPECT_EQ(me.orderbook().asks().at(10).order_count(), 0);

    // Buy should be fully consumed — not resting on bids
    auto bid_it = me.orderbook().bids().find(10);
    if (bid_it != me.orderbook().bids().end()) {
        EXPECT_EQ(bid_it->second.total_shares(), 0);
    }

    EXPECT_EQ(me.volume(), 100);
}

TEST(PartialFill, SellConsumesMultipleBids) {
    MatchingEngine me("TEST");

    // Place bids: 20, 30, 50 shares at $10
    auto b1 = sim::make_limit(Side::BUY, 10, 20, /*oid=*/1, /*coid=*/10);
    auto b2 = sim::make_limit(Side::BUY, 10, 30, /*oid=*/2, /*coid=*/20);
    auto b3 = sim::make_limit(Side::BUY, 10, 50, /*oid=*/3, /*coid=*/30);
    me.process(b1);
    me.process(b2);
    me.process(b3);

    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 100);

    // Sell 100 shares at $10
    auto sell = sim::make_limit(Side::SELL, 10, 100, /*oid=*/100, /*coid=*/999);
    me.process(sell);

    // All bids consumed
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 0);

    EXPECT_EQ(me.volume(), 100);
}

TEST(PartialFill, PartialBuyRestsOnBook) {
    MatchingEngine me("TEST");

    // Place 30 shares of asks at $10
    auto sell = sim::make_limit(Side::SELL, 10, 30, /*oid=*/1);
    me.process(sell);

    // Buy 50 at $10 — fills 30, rests 20 on bid side
    auto buy = sim::make_limit(Side::BUY, 10, 50, /*oid=*/2);
    me.process(buy);

    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 0);
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 20);
    EXPECT_EQ(me.volume(), 30);
}

TEST(PartialFill, PartialSellRestsOnBook) {
    MatchingEngine me("TEST");

    // Place 30 shares of bids at $10
    auto buy = sim::make_limit(Side::BUY, 10, 30, /*oid=*/1);
    me.process(buy);

    // Sell 50 at $10 — fills 30, rests 20 on ask side
    auto sell = sim::make_limit(Side::SELL, 10, 50, /*oid=*/2);
    me.process(sell);

    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 0);
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 20);
    EXPECT_EQ(me.volume(), 30);
}
