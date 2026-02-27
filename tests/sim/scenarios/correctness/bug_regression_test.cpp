#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"
#include "shared/cancel.hpp"
#include "shared/utilities.hpp"
#include <limits>

using namespace exchange;

// ============================================================================
// Bug 1: mkt_to_lim_() — verify MARKET orders get correct price conversion
//
// The original code had:
//   (order.side == Side::BUY) ? UINT64_MAX : 0;   // result discarded!
// The fix assigns: order.price = (side == BUY) ? UINT64_MAX : 0
//
// Test: Send a MARKET BUY when asks exist. If mkt_to_lim_ doesn't set price
// to UINT64_MAX, the buy's original price (0) won't cross the ask.
// ============================================================================
TEST(BugRegression, MktToLimNoOp) {
    // BUG: mkt_to_lim_() discards result — market orders keep price=0
    MatchingEngine me("TEST");

    // Place a sell limit at price 10, qty 50
    auto sell = sim::make_limit(Side::SELL, 10, 50, /*oid=*/1, /*coid=*/100);
    me.process(sell);

    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 50);

    // Send a market buy for 50 shares (price starts at 0 from create_new_order)
    auto mkt_buy = sim::make_market(Side::BUY, 50, /*oid=*/2, /*coid=*/200);
    me.process(mkt_buy);

    // If mkt_to_lim_ works, the buy's price becomes UINT64_MAX, crosses ask at 10,
    // and the ask side at price 10 should be empty after the match.
    // If mkt_to_lim_ is a no-op, price stays 0 and the buy rests on bids at price 0
    // (behind the sentinel), no trade occurs.
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 0)
        << "MARKET BUY did not match ask — mkt_to_lim_ may not be setting price";

    // Volume should reflect the 50-share fill
    EXPECT_EQ(me.volume(), 50);
}

// ============================================================================
// Bug 2: match_DAY_buy_ and match_DAY_sell_ partial fill handling
//
// In the DAY matching methods, when a resting order is only partially filled,
// the code should NOT remove it from the order map or consume it from the
// price level. The IOC/FOK variants correctly gate on (match.payload.qty == 0).
//
// Additional observation: match_DAY_buy_ doesn't call reduce_shares on partial
// fills — the price level's qty_count_ becomes inconsistent with actual order
// quantities.
//
// Test: Place a large sell, then a smaller buy. The sell should have remaining
// qty and still be on the book.
// ============================================================================
TEST(BugRegression, DayMatchKillsPartialFill) {
    // BUG: DAY matching may incorrectly handle partially-filled resting orders
    MatchingEngine me("TEST");

    // Place a SELL LIMIT for 100 shares at $10
    auto sell = sim::make_limit(Side::SELL, 10, 100, /*oid=*/1, /*coid=*/100);
    me.process(sell);

    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 100);
    EXPECT_EQ(me.orderbook().asks().at(10).order_count(), 1);

    // Place a BUY LIMIT for 30 shares at $10 — should partially fill the sell
    auto buy = sim::make_limit(Side::BUY, 10, 30, /*oid=*/2, /*coid=*/200);
    me.process(buy);

    // The sell should have 70 shares remaining on the book
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 70)
        << "Partial fill: sell should have 70 shares remaining";

    // The sell order should still be alive in the ring buffer
    EXPECT_FALSE(me.orderbook().asks().at(10).empty())
        << "Sell order was removed from price level despite partial fill";

    // The buy should be fully consumed (not resting on bids)
    // Price level 10 on bids should not exist or be empty
    auto bid_it = me.orderbook().bids().find(10);
    if (bid_it != me.orderbook().bids().end()) {
        EXPECT_EQ(bid_it->second.total_shares(), 0)
            << "Buy order should be fully filled, not resting on bids";
    }

    // Volume should be 30 (the matched amount)
    EXPECT_EQ(me.volume(), 30);
}

// ============================================================================
// Bug 3: Cancel constructor self-assigns
//
// The Cancel constructor does:
//   order_id(order_id),   // should be oid
//   client_id(client_id)  // should be cid
// This means order_id and client_id get their own uninitialized/default values.
// ============================================================================
TEST(BugRegression, CancelSelfAssign) {
    // BUG: Cancel(cancel_id, oid, cid) self-assigns order_id and client_id
    Cancel c(42, 100, 200);

    EXPECT_EQ(c.cancel_id, 42) << "cancel_id should be correctly assigned";
    EXPECT_EQ(c.order_id, 100)
        << "order_id is self-assigned: expected 100 (oid param) but got garbage/0";
    EXPECT_EQ(c.client_id, 200)
        << "client_id is self-assigned: expected 200 (cid param) but got garbage/0";
}

// ============================================================================
// Bug 4: Return types swapped in utilities.hpp
//
// double_to_uint64_t() is declared returning double — the bit_cast<uint64_t>
// result gets implicitly converted back to double, losing the bit pattern.
// uint64_t_to_double() is declared returning uint64_t — same issue in reverse.
// ============================================================================
TEST(BugRegression, UtilReturnTypeSwap) {
    // BUG: double_to_uint64_t returns double, uint64_t_to_double returns uint64_t

    // IEEE 754: 1.0 is 0x3FF0000000000000 as uint64_t
    auto result = exchange::double_to_uint64_t(1.0);

    // If the return type is correctly uint64_t, result == 0x3FF0000000000000
    // If the return type is double (bug), the bit_cast result gets converted
    // back to double, yielding ~4.607e18 as a double, which is the decimal
    // representation of 0x3FF0000000000000
    static_assert(std::is_same_v<decltype(result), double>,
        "If this fires, double_to_uint64_t was fixed to return uint64_t");

    // With the bug: result is a double, so comparing to uint64_t literal
    // will involve implicit conversion. The round-trip destroys the bit pattern.
    // We test by checking the type is wrong:
    bool returns_double = std::is_same_v<decltype(result), double>;
    EXPECT_FALSE(returns_double)
        << "double_to_uint64_t() returns double instead of uint64_t";

    // Reverse direction
    auto result2 = exchange::uint64_t_to_double(0x3FF0000000000000ULL);
    bool returns_uint64 = std::is_same_v<decltype(result2), uint64_t>;
    EXPECT_FALSE(returns_uint64)
        << "uint64_t_to_double() returns uint64_t instead of double";
}

// ============================================================================
// Bug 5: Silent order drops when price level ring buffer is full
//
// OrderbookRingBuffer::enqueue() silently returns when full (item_count_+1>SIZE).
// RING_BUFFER_SIZE is 1024. Adding a 1025th order at the same price is silently
// dropped with no error.
// ============================================================================
TEST(BugRegression, PriceLevelSilentDrop) {
    // BUG: PriceLevel ring buffer silently drops orders when full (capacity 1024)
    MatchingEngine me("TEST");

    const uint64_t capacity = config::RING_BUFFER_SIZE; // 1024

    // Fill the ring buffer at price 10 on the ask side
    for (uint64_t i = 0; i < capacity; ++i) {
        auto sell = sim::make_limit(Side::SELL, 10, 1, /*oid=*/i + 1);
        me.process(sell);
    }

    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), capacity);
    EXPECT_EQ(me.orderbook().asks().at(10).order_count(), capacity);

    // The buffer should be full now
    EXPECT_TRUE(me.orderbook().asks().at(10).full());

    // Add one more — this will be silently dropped
    auto extra = sim::make_limit(Side::SELL, 10, 1, /*oid=*/capacity + 1);
    me.process(extra);

    // The 1025th order should have been dropped
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), capacity)
        << "1025th order was not silently dropped — ring buffer overflow undetected";
    EXPECT_EQ(me.orderbook().asks().at(10).order_count(), capacity)
        << "Order count should still be 1024 after silent drop";

    // NOTE: There is no error reporting mechanism. The order is simply lost.
    // This is a design issue — the caller has no way to know the order was dropped.
}
