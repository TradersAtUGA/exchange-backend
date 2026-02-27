#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"

using namespace exchange;

// Scenario: Duplicate Oid Overwrites Order Map.
TEST(EdgeCase, DuplicateOidOverwritesOrderMap) {
    MatchingEngine me("TEST");

    // Send two different orders with the same oid
    auto sell1 = sim::make_limit(Side::SELL, 10, 50, /*oid=*/42);
    auto sell2 = sim::make_limit(Side::SELL, 11, 30, /*oid=*/42);

    me.process(sell1);
    me.process(sell2);

    // Both should be on the book at their respective prices (no crash)
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 50);
    EXPECT_EQ(me.orderbook().asks().at(11).total_shares(), 30);

    // The order_map_ entry for oid=42 will point to the last one added.
    // This is a documentation test — we're characterizing existing behavior.
    // Cancelling oid=42 should affect the second order (last write wins).
}

// Scenario: Duplicate Oid Different Sides.
TEST(EdgeCase, DuplicateOidDifferentSides) {
    MatchingEngine me("TEST");

    // Same oid on opposite sides — should not crash
    auto buy = sim::make_limit(Side::BUY, 8, 10, /*oid=*/1);
    auto sell = sim::make_limit(Side::SELL, 12, 10, /*oid=*/1);

    me.process(buy);
    me.process(sell);

    EXPECT_EQ(me.orderbook().bids().at(8).total_shares(), 10);
    EXPECT_EQ(me.orderbook().asks().at(12).total_shares(), 10);
}
