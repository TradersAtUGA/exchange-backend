#pragma once 

#include <string>
#include <cstdint>
#include <chrono>
#include <iostream>

#include "enums.hpp"

using std::uint64_t, std::uint8_t;

namespace exchange {

struct Order {
    
    // DO NOT use this to construct ORDER objects,
    // purely for array allocation purposes 
    Order() 
    : price(0),
      qty(0),
      oid(0),
      side(Side::BUY),
      order_type(OrderType::MARKET),
      tif(TIF::DAY),
      ticker(),
      status(Status::REJECTED)
    {}

    Order(
        Side side,
        OrderType order_type, 
        TIF time_in_force,
        std::string ticker, 
        uint64_t price, 
        uint64_t quantity,
        uint64_t order_id, 
        Status status
    ) :
        side(side),
        order_type(order_type),
        tif(time_in_force),
        ticker(ticker),
        price(price),
        qty(quantity),
        oid(order_id),
        status(Status::NEW)
    {} 


    /**  @brief creates a new order struct */
    Order create_cancel(uint64_t order_id) { 
        Order o = Order(); 
        o.cancel_order_id = order_id; 
        return o; 
    }

    friend std::ostream& operator<<(std::ostream& os, const Order& order) {
        std::cout << "Order Side: "<< static_cast<int>(order.side) << std::endl;
        std::cout << "Order Type: " << static_cast<int>(order.order_type) << std::endl;
        std::cout << "Price: " << order.price << std::endl;
        std::cout << "Quantity: " << order.qty << std::endl;
        std::cout << "Order ID: " << order.oid << std::endl;
        std::cout << "Status code" << static_cast<int>(order.status) << std::endl;
        return os;
    }

    Side side;
    OrderType order_type;
    TIF tif; // time in force
    std::string ticker; // replace with char buffer 
    uint64_t price;
    uint64_t qty; 
    uint64_t oid; // order (specific) id (just sequencer id internally)

    // used internally 
    Status status; // see docs/order.md 

    // NEW fields needed
    uint64_t orig_qty; // original qty passed from broker
    uint64_t coid; // internally assigned (broker) id 

    uint64_t cancel_id; // sequencer assigned cancel id 
    uint64_t cancel_order_id; // cancel order id 
};

}