#include <gtest/gtest.h>
#include "matching-engine/matching_engine.hpp"

using namespace exchange; 

// Limit order shares < Market order shares 
// Limit should be filled completely, market order partially filled
TEST(matching_engine_tests, test_market_diff_qty_buy_then_sell_limit_full_match) {
    MatchingEngine me("XYZ");

    Order buyside = Order::create_new_order(
        Side::BUY, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        5, 
        987654321, 
        0
    );

    Order sellside = Order::create_new_order(
        Side::SELL, 
        OrderType::MARKET, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        0
    );

    Message<Order> buyside_order;
    buyside_order.payload = buyside;

    Message<Order> sellside_order;
    sellside_order.payload = sellside;


    me.process(buyside_order);
    me.process(sellside_order);

    // buyside order should fill entirely and be removed from the orderbook
    EXPECT_EQ(me.orderbook().bids().at(10).order_count(), 0);
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 0);
    EXPECT_THROW(me.orderbook().asks().at(10).order_count(), std::out_of_range);
    EXPECT_EQ(sellside_order.payload.qty, 5); 
}

TEST(matching_engine_tests, test_market_diff_qty_sell_then_buy_limit_full_match) {
    MatchingEngine me("XYZ");

    Order buyside = Order::create_new_order(
        Side::SELL, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        5, 
        987654321, 
        0
    );

    Order sellside = Order::create_new_order(
        Side::BUY, 
        OrderType::MARKET, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        0
    );

    Message<Order> buyside_order;
    buyside_order.payload = buyside;

    Message<Order> sellside_order;
    sellside_order.payload = sellside;


    me.process(buyside_order);
    me.process(sellside_order);

    // buyside order should fill entirely and be removed from the orderbook
    EXPECT_EQ(me.orderbook().asks().at(10).order_count(), 0);
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 0);
    EXPECT_THROW(me.orderbook().bids().at(10).order_count(), std::out_of_range);
    EXPECT_EQ(sellside_order.payload.qty, 5); 
}

// Now Limit shares > Market shares
// Limit order should partially fill, market order is filled entirely
TEST(matching_engine_tests, test_market_diff_qty_buy_then_sell_market_full_match) {
    MatchingEngine me("XYZ");

    Order buyside = Order::create_new_order(
        Side::BUY, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        987654321, 
        0
    );

    Order sellside = Order::create_new_order(
        Side::SELL, 
        OrderType::MARKET, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        5, 
        123456789, 
        0
    );

    Message<Order> buyside_order;
    buyside_order.payload = buyside;

    Message<Order> sellside_order;
    sellside_order.payload = sellside;


    me.process(buyside_order);
    me.process(sellside_order);

    // buyside order should fill entirely and be removed from the orderbook
    EXPECT_EQ(me.orderbook().bids().at(10).order_count(), 1);
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 5);
    EXPECT_THROW(me.orderbook().asks().at(10).order_count(), std::out_of_range);
    EXPECT_EQ(sellside_order.payload.qty, 0);
}

TEST(matching_engine_tests, test_market_diff_qty_sell_then_buy_market_full_match) {
    MatchingEngine me("XYZ");

    Order buyside = Order::create_new_order(
        Side::SELL, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        987654321, 
        0
    );

    Order sellside = Order::create_new_order(
        Side::BUY, 
        OrderType::MARKET, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        5, 
        123456789, 
        0
    );

    Message<Order> buyside_order;
    buyside_order.payload = buyside;

    Message<Order> sellside_order;
    sellside_order.payload = sellside;


    me.process(buyside_order);
    me.process(sellside_order);

    std::cout << me.orderbook() << std::endl;

    // buyside order should fill entirely and be removed from the orderbook
    EXPECT_EQ(me.orderbook().asks().at(10).order_count(), 1);
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 5);
    EXPECT_THROW(me.orderbook().bids().at(10).order_count(), std::out_of_range);
    EXPECT_EQ(sellside_order.payload.qty, 0);
}

// Finally market qty == limit qty 
// Both fill and nothing left on order book 
TEST(matching_engine_tests, test_market_same_qty_mkt_sell) {
    MatchingEngine me("XYZ");

    Order buyside = Order::create_new_order(
        Side::BUY, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        987654321, 
        0
    );

    Order sellside = Order::create_new_order(
        Side::SELL, 
        OrderType::MARKET, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        0
    );

    Message<Order> buyside_order;
    buyside_order.payload = buyside;

    Message<Order> sellside_order;
    sellside_order.payload = sellside;


    me.process(buyside_order);
    me.process(sellside_order);

    EXPECT_EQ(me.orderbook().bids().at(10).order_count(), 0);
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 0);
    EXPECT_THROW(me.orderbook().asks().at(10), std::out_of_range);
    EXPECT_EQ(sellside_order.payload.qty, 0);
}

TEST(matching_engine_tests, test_market_same_qty_mkt_buy) { 
    MatchingEngine me("XYZ");

    Order buyside = Order::create_new_order(
        Side::SELL, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        987654321, 
        0
    );

    Order sellside = Order::create_new_order(
        Side::BUY, 
        OrderType::MARKET, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        0
    );

    Message<Order> buyside_order;
    buyside_order.payload = buyside;

    Message<Order> sellside_order;
    sellside_order.payload = sellside;


    me.process(buyside_order);
    me.process(sellside_order);

    EXPECT_EQ(me.orderbook().asks().at(10).order_count(), 0);
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 0);
    EXPECT_THROW(me.orderbook().bids().at(10), std::out_of_range);
    EXPECT_EQ(sellside_order.payload.qty, 0);
}

