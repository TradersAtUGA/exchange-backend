#include <gtest/gtest.h>
#include "helpers/order_factory.hpp"
#include "config.hpp"

using namespace exchange;

// Scenario: Ring Buffer Exact Capacity.
TEST(Stress, RingBufferExactCapacity) {
    MatchingEngine me("TEST");

    const uint64_t capacity = config::RING_BUFFER_SIZE; // 1024

    // Fill exactly to capacity
    for (uint64_t i = 0; i < capacity; ++i) {
        auto sell = sim::make_limit(Side::SELL, 10, 1, /*oid=*/i + 1);
        me.process(sell);
    }

    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), capacity);
    EXPECT_EQ(me.orderbook().asks().at(10).order_count(), capacity);
    EXPECT_TRUE(me.orderbook().asks().at(10).full());
}

// Scenario: Ring Buffer Overflow.
TEST(Stress, RingBufferOverflow) {
    MatchingEngine me("TEST");

    const uint64_t capacity = config::RING_BUFFER_SIZE;

    for (uint64_t i = 0; i < capacity; ++i) {
        auto sell = sim::make_limit(Side::SELL, 10, 1, /*oid=*/i + 1);
        me.process(sell);
    }

    // Overflow: the 1025th order will be silently dropped by enqueue()
    auto overflow = sim::make_limit(Side::SELL, 10, 1, /*oid=*/capacity + 1);
    me.process(overflow);

    // Still only capacity orders
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), capacity)
        << "Ring buffer overflow was not silently dropped";

    // Verify matching still works after saturation
    auto buy = sim::make_limit(Side::BUY, 10, 10, /*oid=*/capacity + 2);
    me.process(buy);

    EXPECT_EQ(me.volume(), 10);
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), capacity - 10);
}

// Scenario: Ring Buffer Bid Side Saturation.
TEST(Stress, RingBufferBidSideSaturation) {
    MatchingEngine me("TEST");

    const uint64_t capacity = config::RING_BUFFER_SIZE;

    for (uint64_t i = 0; i < capacity; ++i) {
        auto buy = sim::make_limit(Side::BUY, 10, 1, /*oid=*/i + 1);
        me.process(buy);
    }

    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), capacity);
    EXPECT_TRUE(me.orderbook().bids().at(10).full());

    // Overflow
    auto overflow = sim::make_limit(Side::BUY, 10, 1, /*oid=*/capacity + 1);
    me.process(overflow);

    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), capacity);
}
