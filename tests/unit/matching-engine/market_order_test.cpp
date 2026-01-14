#include <gtest/gtest.h>
#include "matching-engine/matching_engine.hpp"

using namespace exchange; 

TEST(MATCH_ENG_market_order_test, test_diff_qty) {
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
        OrderType::MARKET,
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

    sellside.order_type = OrderType::LIMIT;
    buyside.order_type = OrderType::MARKET;

    em.process(sellside);
    em.process(buyside);

    // buyside order should fill entirely and be removed from the orderbook
    EXPECT_EQ(me.orderbook().bids().at(10).order_count(), 0);

    // no orders should exist on the asking side since the market order does not rest
    EXPECT_THROW(me.orderbook().asks().at(10).order_count(), std::out_of_range);

    // expect the price level from a fully filled ask to be empty now 
    EXPECT_EQ(em.orderbook().asks().at(10).order_count(), 0);

    // expect the market order is never added to the orderbook but is partially fillled
    //EXPECT_THROW(em.orderbook().bids().at(10).order_count(), std::out_of_range);
    
    EXPECT_EQ(em.ledger().size(), 1);
    EXPECT_EQ(em.ledger()[0].qty, 5);
    EXPECT_EQ(em.ledger()[0].price, 10);

}

TEST(MATCH_ENG_market_order_test, test_same_qty) {
    
}

