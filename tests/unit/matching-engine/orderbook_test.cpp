#include <gtest/gtest.h>
#include "matching-engine/matching_engine.hpp"

using namespace exchange; 

TEST(orderbook_tests, empty_ob_add_sell_limit_order) {
    MatchingEngine me("XYZ");

    Order sellside_ = Order::create_new_order(
        Side::SELL, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        1
    );

    Message<Order> sellside;
    sellside.payload = sellside_;

    me.process(sellside);

    // check order exists on order book since no matching occurs
    EXPECT_EQ(me.orderbook().asks().at(10).order_count(), 1);
    EXPECT_EQ(me.orderbook().asks().at(10).total_shares(), 10);
    EXPECT_EQ(me.orderbook().asks().at(10).empty(), 0);
}

TEST(orderbook_tests, empty_ob_add_buy_limit_order) {
    MatchingEngine me("XYZ");

    Order sellside_ = Order::create_new_order(
        Side::BUY, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        1
    );

    sellside_.price = 10; 

    Message<Order> sellside;
    sellside.payload = sellside_;

    me.process(sellside);
    auto whatever = me.orderbook().bids();

    for (const auto [k, v] : whatever) { 
        std::cout << k << " " << v << " " << std::endl;
    }

    // check order exists on order book since no matching occurs
    EXPECT_EQ(me.orderbook().bids().at(10).order_count(), 1);
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 10);
    EXPECT_EQ(me.orderbook().bids().at(10).empty(), 0);
} 

TEST(orderbook_tests, empty_ob_add_sell_market_order) {
    MatchingEngine me("XYZ");

    Order sellside_ = Order::create_new_order(
        Side::SELL, 
        OrderType::MARKET, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        1
    );

    Message<Order> sellside;
    sellside.payload = sellside_;
    me.process(sellside);

    // methods calling at should throw since no orders exist on the orderbook yet
    EXPECT_THROW(me.orderbook().asks().at(10).order_count(), std::out_of_range);
    EXPECT_THROW(me.orderbook().asks().at(10).total_shares(), std::out_of_range);
    EXPECT_THROW(me.orderbook().asks().at(10).empty(), std::out_of_range);
}


TEST(orderbook_tests, empty_ob_add_buy_market_order) {
    MatchingEngine me("XYZ");

    Order sellside_ = Order::create_new_order(
        Side::BUY, 
        OrderType::MARKET, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        1
    );

    Message<Order> sellside;
    sellside.payload = sellside_;
    me.process(sellside);

    // methods calling at should throw since no orders exist on the orderbook yet
    EXPECT_THROW(me.orderbook().bids().at(10).order_count(), std::out_of_range);
    EXPECT_THROW(me.orderbook().bids().at(10).total_shares(), std::out_of_range);
    EXPECT_THROW(me.orderbook().bids().at(10).empty(), std::out_of_range);
}

TEST(orderbook_tests, cancel) {
    MatchingEngine me("XYZ");
 
    Order sellside_ = Order::create_new_order(
        Side::BUY, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        52, 
        1
    );

    Message<Order> sellside;
    sellside.payload = sellside_;

    me.process(sellside);

    Order cancel_ = Order::create_cancel(
        52,
        1
    );

    Message<Order> cancel(cancel_);

    /*
    1. this method should be doing payload checks within it since we are 
    using order methods to represent cancel orders
    2. Rewrite this method to make sure that it is actually cancelling based
    on the definition of cancel orders not the original order struct definition 
    */
    me.cancel(cancel);

    std::cout << &me.orderbook().bids().at(10).front() << std::endl;

    EXPECT_EQ(me.orderbook().bids().at(10).front().payload.status, Status::CANCELED); // order should be marked dead but still exist in the ob
    EXPECT_EQ(me.orderbook().bids().at(10).total_shares(), 0);
    EXPECT_EQ(me.orderbook().bids().at(10).order_count(), 0);
    EXPECT_THROW(me.orderbook().orders().at(sellside.payload.oid), std::out_of_range); // order ptr should not exist in orderbook anymore
}

TEST(orderbook_tests, best_ask) {

    MatchingEngine me("XYZ");

    Order sellside_ = Order::create_new_order(
        Side::SELL, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        1
    );

    Message<Order> sellside;
    sellside.payload = sellside_;
    me.process(sellside);

    EXPECT_EQ(me.orderbook().best_ask(), 10);
}

TEST(orderbook_tests, best_bid) {

    MatchingEngine me("XYZ");

    Order sellside_ = Order::create_new_order(
        Side::BUY, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        1
    );

    Message<Order> sellside;
    sellside.payload = sellside_;
    me.process(sellside);

    EXPECT_EQ(me.orderbook().best_bid(), 10);
}

TEST(orderbook_tests, bid_shares_limit_ge) {
    MatchingEngine me("XYZ");

    Order sellside_ = Order::create_new_order(
        Side::BUY, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        1
    );

    Message<Order> sellside;
    sellside.payload = sellside_;


    me.process(sellside);

    EXPECT_EQ(me.orderbook().check_bid_shares_limit_ge(10, 5), 1);
}

TEST(orderbook_tests, bid_shares_ge) {
    MatchingEngine me("XYZ");

   Order sellside_ = Order::create_new_order(
        Side::BUY, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        1
    );

    Message<Order> sellside;
    sellside.payload = sellside_;


    me.process(sellside);

    EXPECT_EQ(me.orderbook().check_bid_shares_ge(10), 1);
}

TEST(orderbook_tests, ask_shares_limit_ge) {
    MatchingEngine me("XYZ");

  Order sellside_ = Order::create_new_order(
        Side::SELL, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        1
    );

    Message<Order> sellside;
    sellside.payload = sellside_;


    me.process(sellside);

    EXPECT_EQ(me.orderbook().check_ask_shares_limit_ge(10, 25), 1);
}

TEST(orderbook_tests, ask_shares_ge) {
    MatchingEngine me("XYZ");

    Order sellside_ = Order::create_new_order(
        Side::SELL, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        1
    );

    Message<Order> sellside;
    sellside.payload = sellside_;

    me.process(sellside);

    EXPECT_EQ(me.orderbook().check_ask_shares_ge(10), 1);
}