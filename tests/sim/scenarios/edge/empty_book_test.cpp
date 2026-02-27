#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"
#include "assertions/orderbook_invariants.hpp"

using namespace exchange;

// Scenario: Aggressive Buy On Empty Book.
TEST(EdgeCase, AggressiveBuyOnEmptyBook) {
    MatchingEngine me("TEST");

    // Aggressive buy on empty book — only sentinel ask at UINT64_MAX
    auto buy = sim::make_limit(Side::BUY, 100, 50, /*oid=*/1);
    me.process(buy);

    // Should rest on bids, no crash
    EXPECT_EQ(me.orderbook().bids().at(100).total_shares(), 50);
    EXPECT_EQ(me.volume(), 0);

    std::string inv = sim::check_orderbook_invariants(me.orderbook());
    EXPECT_TRUE(inv.empty()) << inv;
}

// Scenario: Aggressive Sell On Empty Book.
TEST(EdgeCase, AggressiveSellOnEmptyBook) {
    MatchingEngine me("TEST");

    auto sell = sim::make_limit(Side::SELL, 100, 50, /*oid=*/1);
    me.process(sell);

    EXPECT_EQ(me.orderbook().asks().at(100).total_shares(), 50);
    EXPECT_EQ(me.volume(), 0);

    std::string inv = sim::check_orderbook_invariants(me.orderbook());
    EXPECT_TRUE(inv.empty()) << inv;
}

// Scenario: Market Order On Empty Book.
TEST(EdgeCase, MarketOrderOnEmptyBook) {
    MatchingEngine me("TEST");

    // Market buy — should not crash, should not match sentinels
    auto mkt = sim::make_market(Side::BUY, 50, /*oid=*/1);
    me.process(mkt);

    EXPECT_EQ(me.volume(), 0);

    std::string inv = sim::check_orderbook_invariants(me.orderbook());
    EXPECT_TRUE(inv.empty()) << inv;
}

// Scenario: Market Sell On Empty Book.
TEST(EdgeCase, MarketSellOnEmptyBook) {
    MatchingEngine me("TEST");

    auto mkt = sim::make_market(Side::SELL, 50, /*oid=*/1);
    me.process(mkt);

    EXPECT_EQ(me.volume(), 0);

    std::string inv = sim::check_orderbook_invariants(me.orderbook());
    EXPECT_TRUE(inv.empty()) << inv;
}
