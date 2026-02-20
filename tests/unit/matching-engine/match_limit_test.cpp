#include <gtest/gtest.h>
#include "matching-engine/matching_engine.hpp"
#include <ostream>

// Orders can be assumed to be validated at this point

using namespace exchange; 

TEST(matching_engine_tests, test_limit_diff_qty_buy_then_sell) {
    MatchingEngine me("XYZ");

    Order buyside_ = Order::create_new_order(
        Side::BUY, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        987654321, 
        1
    );

    Order sellside_ = Order::create_new_order(
        Side::SELL, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"),
        10, 
        5, 
        123456789, 
        0
    );

    Message<Order> sellside;
    sellside.payload = sellside_;

    Message<Order> buyside;
    buyside.payload = buyside_;

    me.process(buyside);
    me.process(sellside);

    EXPECT_EQ(me.orderbook().bids().at(10).order_count(), 1);
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 5);
}

TEST(matching_engine_tests, test_limit_same_qty) {

    MatchingEngine me("XYZ");
    MatchingEngine em("XYZ");

    Order buyside_ = Order::create_new_order(
        Side::BUY, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        987654321, 
        0
    );

    Order sellside_ = Order::create_new_order(
        Side::SELL, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        0
    );

    Message<Order> buyside;
    buyside.payload = buyside_;

    Message<Order> sellside; 
    sellside.payload = sellside_; 
    
    me.process(buyside);
    me.process(sellside);
    // expect same result regardless of insert order 
    // since qty is the same on each side 

    sellside.payload.qty = 10;
    buyside.payload.qty = 10;

    em.process(sellside);
    em.process(buyside);


    // make sure orders erased from order book 
    EXPECT_EQ(me.orderbook().bids().at(10).order_count(), 0);
    EXPECT_EQ(em.orderbook().asks().at(10).order_count(),0);

    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 0);
    EXPECT_EQ(em.orderbook().asks().at(10).total_shares(), 0);
}