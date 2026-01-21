#include <gtest/gtest.h>
#include "matching-engine/matching_engine.hpp"
#include <ostream>

// Orders can be assumed to be validated at this point

using namespace exchange; 

TEST(matching_engine_tests, test_limit_diff_qty) {

    MatchingEngine me("XYZ");
    MatchingEngine em("XYZ");

    Order buyside = Order(
        Side::BUY, 
        OrderType::LIMIT,
        TIF::DAY,
        "XYZ",
        10, // price
        5, // qty
        10,
        987654321,
        10,
        1
    );

     Order sellside = Order(
        Side::SELL, 
        OrderType::LIMIT,
        TIF::DAY,
        "XYZ",
        10, // price
        10, // qty 
        10,
        123456789,
        10,
        1
    );

    me.process(buyside);
    me.process(sellside);

    sellside.qty = 5;
    buyside.qty = 10;

    em.process(sellside);
    em.process(buyside);

    EXPECT_EQ(me.orderbook().bids().at(10).order_count(), 0);
    EXPECT_EQ(me.orderbook().asks().at(10).order_count(), 1);
    EXPECT_EQ(em.orderbook().bids().at(10).order_count(), 1);
    EXPECT_EQ(em.orderbook().asks().at(10).order_count(), 0);

     // check if ledger contains trade
    EXPECT_EQ(me.ledger().size(), 1);
    EXPECT_EQ(em.ledger().size(), 1);

    // check if ledgers properly generated trade
    EXPECT_EQ(me.ledger()[0].qty, 5);  
    EXPECT_EQ(me.ledger()[0].price, 10);

    EXPECT_EQ(em.ledger()[0].qty, 5);
    EXPECT_EQ(em.ledger()[0].price, 10);
}

TEST(matching_engine_tests, test_limit_same_qty) {

    MatchingEngine me("XYZ");
    MatchingEngine em("XYZ");

    Order buyside = Order(
        Side::BUY, 
        OrderType::LIMIT,
        TIF::DAY,
        "XYZ",
        10, // price
        10,
        10,
        987654321,
        10,
        1
    );

     Order sellside = Order(
        Side::SELL, 
        OrderType::LIMIT,
        TIF::DAY,
        "XYZ",
        10, // price
        10,
        10,
        123456789,
        10,
        1
    );
    
    me.process(buyside);
    me.process(sellside);
    // expect same result regardless of insert order 
    // since qty is the same on each side 

    sellside.qty = 10;
    buyside.qty = 10;

    em.process(sellside);
    em.process(buyside);

    // check if ledger contains trade
    EXPECT_EQ(me.ledger().size(), 1);
    EXPECT_EQ(em.ledger().size(), 1);

    // check if ledgers properly generated trade
    EXPECT_EQ(me.ledger()[0].qty, 10);  
    EXPECT_EQ(me.ledger()[0].price, 10);

    EXPECT_EQ(em.ledger()[0].qty, 10);
    EXPECT_EQ(em.ledger()[0].price, 10);

    // make sure orders erased from order book 
    EXPECT_EQ(me.orderbook().bids().at(10).order_count(), 0);
    EXPECT_EQ(em.orderbook().asks().at(10).order_count(),0);

    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 0);
    EXPECT_EQ(em.orderbook().asks().at(10).total_shares(), 0);
}