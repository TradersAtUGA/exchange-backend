#include <gtest/gtest.h>
#include "matching-engine/matching_engine.hpp"

using namespace exchange; 

TEST(orderbook_tests, empty_ob_add_sell_limit_order) {
    MatchingEngine me("XYZ");

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

    me.process(sellside);

    // check order exists on order book since no matching occurs
    EXPECT_EQ(me.orderbook().asks().at(10).order_count(), 1);
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 10);
    EXPECT_EQ(me.orderbook().asks().at(10).empty(), 0);
}

TEST(orderbook_tests, empty_ob_add_buy_limit_order) {
    MatchingEngine me("XYZ");

    Order sellside = Order(
        Side::BUY, 
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

    me.process(sellside);

    // check order exists on order book since no matching occurs
    EXPECT_EQ(me.orderbook().bids().at(10).order_count(), 1);
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 10);
    EXPECT_EQ(me.orderbook().bids().at(10).empty(), 0);
} 

TEST(orderbook_tests, empty_ob_add_sell_market_order) {
    MatchingEngine me("XYZ");

    Order sellside = Order( // should die with no fills and no resting 
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

    me.process(sellside);

    // methods calling at should throw since no orders exist on the orderbook yet
    EXPECT_THROW(me.orderbook().asks().at(10).order_count(), std::out_of_range);
    EXPECT_THROW(me.orderbook().asks().at(10).total_shares(), std::out_of_range);
    EXPECT_THROW(me.orderbook().asks().at(10).empty(), std::out_of_range);
}


TEST(orderbook_tests, empty_ob_add_buy_market_order) {
    MatchingEngine me("XYZ");

    Order sellside = Order( // should die with no fills and no resting 
        Side::BUY, 
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

    me.process(sellside);

    // methods calling at should throw since no orders exist on the orderbook yet
    EXPECT_THROW(me.orderbook().bids().at(10).order_count(), std::out_of_range);
    EXPECT_THROW(me.orderbook().bids().at(10).total_shares(), std::out_of_range);
    EXPECT_THROW(me.orderbook().bids().at(10).empty(), std::out_of_range);
}

TEST(orderbook_tests, cancel) {
    MatchingEngine me("XYZ");

    Order sellside = Order(
        Side::BUY, 
        OrderType::LIMIT, // let order rest on ob
        TIF::DAY,
        "XYZ",
        10, // price
        10, // qty 
        10,
        123456789,
        10,
        1
    );

    me.process(sellside);
    me.cancel(sellside);

    EXPECT_EQ(me.orderbook().bids().at(10).front().status, 0); // order should be marked dead but still exist in the ob
    EXPECT_THROW(me.orderbook().orders().at(sellside.oid), std::out_of_range); // order ptr should not exist in orderbook anymore
}

TEST(orderbook_tests, best_ask) {

    MatchingEngine me("XYZ");

    Order o = Order(
        Side::SELL, 
        OrderType::LIMIT, // let order rest on ob
        TIF::DAY,
        "XYZ",
        10, // price
        10, // qty 
        10,
        123456789,
        10,
        1
    );  

    me.process(o);

    EXPECT_EQ(me.orderbook().best_ask(), 10);
}

TEST(orderbook_tests, best_bid) {

    MatchingEngine me("XYZ");

    Order o = Order(
        Side::BUY, 
        OrderType::LIMIT, // let order rest on ob
        TIF::DAY,
        "XYZ",
        10, // price
        10, // qty 
        10,
        123456789,
        10,
        1
    );  

    me.process(o);

    EXPECT_EQ(me.orderbook().best_bid(), 10);
}

TEST(orderbook_tests, bid_shares_limit_ge) {
    MatchingEngine me("XYZ");

    Order o = Order(
        Side::BUY, 
        OrderType::LIMIT, // let order rest on ob
        TIF::DAY,
        "XYZ",
        10, // price
        10, // qty 
        10,
        123456789,
        10,
        1
    );  

    me.process(o);

    EXPECT_EQ(me.orderbook().check_bid_shares_limit_ge(10, 5), 1);
}

TEST(orderbook_tests, bid_shares_ge) {
    MatchingEngine me("XYZ");

    Order o = Order(
        Side::BUY, 
        OrderType::LIMIT, // let order rest on ob
        TIF::DAY,
        "XYZ",
        10, // price
        10, // qty 
        10,
        123456789,
        10,
        1
    );  

    me.process(o);

    EXPECT_EQ(me.orderbook().check_bid_shares_ge(10), 1);
}

TEST(orderbook_tests, ask_shares_limit_ge) {
    MatchingEngine me("XYZ");

    Order o = Order(
        Side::SELL, 
        OrderType::LIMIT, // let order rest on ob
        TIF::DAY,
        "XYZ",
        10, // price
        10, // qty 
        10,
        123456789,
        10,
        1
    );  

    me.process(o);

    EXPECT_EQ(me.orderbook().check_ask_shares_limit_ge(10, 25), 1);
}

TEST(orderbook_tests, ask_shares_ge) {
    MatchingEngine me("XYZ");

    Order o = Order(
        Side::SELL, 
        OrderType::LIMIT, // let order rest on ob
        TIF::DAY,
        "XYZ",
        10, // price
        10, // qty 
        10,
        123456789,
        10,
        1
    );  

    me.process(o);

    EXPECT_EQ(me.orderbook().check_ask_shares_ge(10), 1);
}