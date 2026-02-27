#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"

using namespace exchange;

TEST(FOK, RejectWhenInsufficientShares) {
    MatchingEngine me("TEST");

    // Place 80 shares of asks at $10
    auto s1 = sim::make_limit(Side::SELL, 10, 80, /*oid=*/1);
    me.process(s1);

    // FOK buy for 100 at $10 — not enough shares, reject entirely
    auto fok = sim::make_limit_fok(Side::BUY, 10, 100, /*oid=*/100);
    me.process(fok);

    // No trade should occur
    EXPECT_EQ(me.volume(), 0);

    // Orderbook should be unchanged
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 80);

    // FOK should not rest
    auto bid_it = me.orderbook().bids().find(10);
    if (bid_it != me.orderbook().bids().end()) {
        EXPECT_EQ(bid_it->second.total_shares(), 0);
    }
}

TEST(FOK, FillWhenExactSharesAvailable) {
    MatchingEngine me("TEST");

    // Place exactly 100 shares of asks at $10
    auto s1 = sim::make_limit(Side::SELL, 10, 60, /*oid=*/1);
    auto s2 = sim::make_limit(Side::SELL, 10, 40, /*oid=*/2);
    me.process(s1);
    me.process(s2);

    // FOK buy for 100 at $10 — exactly enough
    auto fok = sim::make_limit_fok(Side::BUY, 10, 100, /*oid=*/100);
    me.process(fok);

    EXPECT_EQ(me.volume(), 100);
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 0);
}

TEST(FOK, FillWhenMoreThanEnoughShares) {
    MatchingEngine me("TEST");

    // Place 150 shares at $10
    auto s1 = sim::make_limit(Side::SELL, 10, 150, /*oid=*/1);
    me.process(s1);

    // FOK buy for 100 — enough, fills 100
    auto fok = sim::make_limit_fok(Side::BUY, 10, 100, /*oid=*/100);
    me.process(fok);

    EXPECT_EQ(me.volume(), 100);
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 50);
}

TEST(FOK, SellSideFOKReject) {
    MatchingEngine me("TEST");

    // Place 50 shares of bids at $10
    auto buy = sim::make_limit(Side::BUY, 10, 50, /*oid=*/1);
    me.process(buy);

    // FOK sell for 100 at $10 — not enough
    auto fok = sim::make_limit_fok(Side::SELL, 10, 100, /*oid=*/100);
    me.process(fok);

    EXPECT_EQ(me.volume(), 0);
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 50);
}

TEST(FOK, LimitPriceRestriction) {
    MatchingEngine me("TEST");

    // Place asks: 50 at $10, 50 at $11
    auto s1 = sim::make_limit(Side::SELL, 10, 50, /*oid=*/1);
    auto s2 = sim::make_limit(Side::SELL, 11, 50, /*oid=*/2);
    me.process(s1);
    me.process(s2);

    // FOK buy for 100 at $10 — only 50 available at $10 or better
    auto fok = sim::make_limit_fok(Side::BUY, 10, 100, /*oid=*/100);
    me.process(fok);

    // Should be rejected — not enough shares at limit price
    EXPECT_EQ(me.volume(), 0);
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 50);
    EXPECT_EQ(me.orderbook().asks().at(11).total_shares(), 50);
}
