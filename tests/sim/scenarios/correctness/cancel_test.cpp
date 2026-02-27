#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"

using namespace exchange;

// Scenario: Cancel Resting Sell Then Buy Crosses Empty.
TEST(CancelOrder, CancelRestingSellThenBuyCrossesEmpty) {
    MatchingEngine me("TEST");

    // Place a sell at $10
    auto sell = sim::make_limit(Side::SELL, 10, 50, /*oid=*/42, /*coid=*/100);
    me.process(sell);

    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 50);

    // Cancel oid=42 using the Message<Order> cancel path
    auto cancel_msg = sim::make_cancel(/*cancel_order_id=*/42, /*cancel_id=*/1);
    me.cancel(cancel_msg);

    // The order's shares should be reduced from the price level
    // Note: cancel marks status=CANCELED and reduces shares,
    // but the order remains in the ring buffer until consumed during matching
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 0)
        << "Cancel should reduce share count to 0";

    // Send a crossing buy — should not match the cancelled order
    auto buy = sim::make_limit(Side::BUY, 10, 50, /*oid=*/100, /*coid=*/200);
    me.process(buy);

    // The buy should rest on bids since there's nothing real to match at $10
    // (cancelled order still in ring buffer but with status=CANCELED)
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 50)
        << "Buy should rest since the sell was cancelled";
}

// Scenario: Cancel Non Existent Order No Crash.
TEST(CancelOrder, CancelNonExistentOrderNoCrash) {
    MatchingEngine me("TEST");

    // Cancel an order that was never placed — should not crash
    auto cancel_msg = sim::make_cancel(/*cancel_order_id=*/999, /*cancel_id=*/1);
    me.cancel(cancel_msg);

    // Verify engine is still usable
    auto sell = sim::make_limit(Side::SELL, 10, 10, /*oid=*/1);
    me.process(sell);
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 10);
}

// Scenario: Double Cancel No Crash.
TEST(CancelOrder, DoubleCancelNoCrash) {
    MatchingEngine me("TEST");

    auto sell = sim::make_limit(Side::SELL, 10, 50, /*oid=*/1);
    me.process(sell);

    // Cancel once
    auto cancel1 = sim::make_cancel(/*cancel_order_id=*/1, /*cancel_id=*/1);
    me.cancel(cancel1);

    // Cancel again — should be no-op, no crash
    auto cancel2 = sim::make_cancel(/*cancel_order_id=*/1, /*cancel_id=*/2);
    me.cancel(cancel2);

    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 0);
}

// Scenario: Cancel Bid Order.
TEST(CancelOrder, CancelBidOrder) {
    MatchingEngine me("TEST");

    auto buy = sim::make_limit(Side::BUY, 10, 50, /*oid=*/1);
    me.process(buy);

    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 50);

    auto cancel_msg = sim::make_cancel(/*cancel_order_id=*/1, /*cancel_id=*/1);
    me.cancel(cancel_msg);

    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 0);
}
