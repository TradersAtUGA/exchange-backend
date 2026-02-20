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
    : 
        side(Side::BUY),
        ord_type(OrderType::LIMIT),
        tif(TIF::DAY),
        ticker(""),
        price(0),
        qty(0),
        orig_qty(0),
        status(Status::NEW),
        oid(0),
        cancel_order_id(0),
        coid(0),
        cancel_id(0)
    {}


    /**  @brief creates a new order struct */
    static Order create_cancel(uint64_t order_id, uint64_t cancel_id) { 
        Order o = Order(); 
        o.cancel_order_id = order_id; 
        o.cancel_id = cancel_id;
        o.plt = PayloadType::CANCEL;
        return o; 
    }


    /** 
     * @brief creates a new order
     * 
     * @param side the side of this order
     * @param ord_type the type of this order
     * @param time_in_force time in force of this order
     * @param ticker the ticker that this order is being placed 
     * for @warning the ticker must be at most 6 chars including the 
     * null terminator 
     * @param price the limit price of this order for market orders
     * @param quantity the amount of shares this order placed
     * @param order_id sequencer assigned value @warning do not manually
     * assign this value
     */
    static Order create_new_order(
        Side side,
        OrderType ord_type, 
        TIF time_in_force,
        char * ticker,
        uint64_t price, 
        uint64_t quantity,
        uint64_t order_id,
        uint64_t client_order_id
    ) {
        Order o;
        o.side = side; 
        o.ord_type = ord_type;
        o.tif = time_in_force;
        strncpy(o.ticker, ticker, 6);
        o.price = price; 
        o.qty = quantity;
        o.oid = order_id;
        o.orig_qty = quantity;
        o.status = Status::NEW;
        o.plt = PayloadType::ORDER;
        o.coid = client_order_id;
        return o;
    }

    friend std::ostream& operator<<(std::ostream& os, const Order& order) {
        std::cout << "Order Side: "<< static_cast<int>(order.side) << std::endl;
        std::cout << "Order Type: " << static_cast<int>(order.ord_type) << std::endl;
        std::cout << "Price: " << order.price << std::endl;
        std::cout << "Quantity: " << order.qty << std::endl;
        std::cout << "Order ID: " << order.oid << std::endl;
        std::cout << "Status: " << static_cast<int>(order.status) << std::endl;
        return os;
    }

    // Explicitly defined fields 
    Side side;
    OrderType ord_type;
    TIF tif; // time in force
    char ticker[6];
    uint64_t price;
    uint64_t qty; 
    uint64_t orig_qty; // original qty passed from broker
    Status status; // used internally, see docs/order.md 

    // Method specific
    PayloadType plt; // describes actual payload type (see enums.hpp)

    // Assigned internally 
    uint64_t oid; // order (specific) id (just sequencer id internally)
    uint64_t cancel_order_id; // cancel order id 

    // Assigned by broker 
    uint64_t coid; // internally assigned (broker) id 
    uint64_t cancel_id; // sequencer assigned cancel id 

};

}