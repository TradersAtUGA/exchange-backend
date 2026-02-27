#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"

using namespace exchange;

TEST(IOC, PartialFillDoesNotRest) {
    MatchingEngine me("TEST");

    // Place 60 shares of asks at $10
    auto s1 = sim::make_limit(Side::SELL, 10, 30, /*oid=*/1);
    auto s2 = sim::make_limit(Side::SELL, 10, 30, /*oid=*/2);
    me.process(s1);
    me.process(s2);

    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 60);

    // Send IOC buy for 100 shares at $10 — should fill 60 and discard 40
    auto ioc = sim::make_limit_ioc(Side::BUY, 10, 100, /*oid=*/100);
    me.process(ioc);

    // 60 shares should have been filled
    EXPECT_EQ(me.volume(), 60);

    // Ask side at $10 should be empty
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 0);

    // The remaining 40 should NOT rest on the bid side
    auto bid_it = me.orderbook().bids().find(10);
    if (bid_it != me.orderbook().bids().end()) {
        EXPECT_EQ(bid_it->second.total_shares(), 0)
            << "IOC order should not rest on bid side";
    }
}

TEST(IOC, NoMatchDiscardsEntireOrder) {
    MatchingEngine me("TEST");

    // Empty book (only sentinels) — IOC buy should find nothing
    auto ioc = sim::make_limit_ioc(Side::BUY, 10, 50, /*oid=*/1);
    me.process(ioc);

    EXPECT_EQ(me.volume(), 0);

    // Order should not be on the book
    auto bid_it = me.orderbook().bids().find(10);
    if (bid_it != me.orderbook().bids().end()) {
        EXPECT_EQ(bid_it->second.total_shares(), 0);
    }
}

TEST(IOC, SellSideIOC) {
    MatchingEngine me("TEST");

    // Place 40 shares of bids at $10
    auto buy = sim::make_limit(Side::BUY, 10, 40, /*oid=*/1);
    me.process(buy);

    // IOC sell for 100 at $10 — fills 40, discards 60
    auto ioc = sim::make_limit_ioc(Side::SELL, 10, 100, /*oid=*/2);
    me.process(ioc);

    EXPECT_EQ(me.volume(), 40);
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 0);

    // Should not rest on asks
    auto ask_it = me.orderbook().asks().find(10);
    if (ask_it != me.orderbook().asks().end()) {
        EXPECT_EQ(ask_it->second.total_shares(), 0);
    }
}
