#include <gtest/gtest.h>
#include "matching-engine/matching_engine.hpp"

using namespace exchange; 

TEST(matching_engine_tests, test_market_diff_qty_buy_then_sell) {
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
}

TEST(matching_engine_tests, test_market_diff_qty_sell_then_buy) {
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
}

TEST(matching_engine_tests, test_market_same_qty) {
    
}

