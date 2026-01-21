#pragma once

#include <map>
#include <string>
#include <utility>
#include <cstdint>
#include <limits>

#include "shared/order.hpp"
#include "shared/trade.hpp"
#include "shared/enums.hpp"
#include "shared/pricelevel.hpp"
#include "shared/cancel.hpp"
#include "config.hpp"

using std::uint64_t, std::uint8_t; 

namespace exchange {

class OrderBook {
public:

    static constexpr uint64_t MAX_PRICE = UINT64_MAX;
    static constexpr uint64_t MIN_PRICE = 0;

    explicit OrderBook(const std::string& ticker) : ticker_(ticker) {
        // add a ask with max possible value just to init the orderbook
        Order max_ask;
        max_ask.price = std::numeric_limits<uint64_t>::max();
        max_ask.side = Side::SELL;
        Message<Order> max_ask_msg = {max_ask, 0, 0};
        add_order(max_ask_msg);

        Order min_ask;
        min_ask.price = 0;
        min_ask.side = Side::BUY;
        Message<Order> min_bid_msg = {min_ask, 0, 0};
        add_order(min_bid_msg);
    };
    
    /**
     * @brief adds an order to the orderbook
     * 
     * @param order order to add
     */
    void add_order(Order order);
    // NEW
    void add_order(Message<Order> msg); 

    /**
     * @brief cancels an order that was already placed
     * 
     * @param order order to remove
     */
    void cancel_order(const Order& order);

    // new (use can.payload.order_id)
    void cancel_order(const Message<Cancel>& msg); 

    /**
     * @brief removes the order pointer in the orders_
     * variable
     * 
     * @param id the id of the order, must exist 
     */
    void remove_order_ptr(uint64_t id); 



    /** @brief operator overload for std::cout */
    friend std::ostream& operator<<(std::ostream& os, const OrderBook& ob);

    /** @brief return orderbook ticker */
    std::string& ticker() { return ticker_; }
    const std::string& ticker() const { return ticker_; } 

    /** @brief return orderbook bids */
    std::map<uint64_t, PriceLevel, std::greater<uint64_t>>& bids() { return bids_; }
    const std::map<uint64_t, PriceLevel, std::greater<uint64_t>>& bids() const { return bids_; }

    /** @brief return orderbook asks */
    std::map<uint64_t, PriceLevel>& asks() { return asks_; }
    const std::map<uint64_t, PriceLevel>& asks() const { return asks_; }

    /** @brief return map of order id's to pointers of orders for all current orders */
    std::map<uint64_t, Order*>& orders() { return orders_; } 
    const std::map<uint64_t, Order*>& orders() const { return orders_; }

    /** @brief returns the best asking price */
    uint64_t best_ask() const { return asks_.begin()->first; } // TODO(vikas): handle edge case if the order is canceled and the best ask is not updated to account for this need to check share count at every price level 

    /** @brief returns the best bidding price */
    uint64_t best_bid() const { return bids_.begin()->first; }

    /**
     * @brief checks asking side to see if a given amount 
     * of shares are available 
     * 
     * @param shares the number of shares to check
     * @return 1 if there are more or equivalent number of 
     * asking shares, else 0
     */
    uint8_t check_ask_shares_ge(uint64_t shares) const; 

     /**
     * @brief checks bidding side to see if a given amount 
     * of shares are available 
     * 
     * @param shares the number of shares to check
     * @return 1 if there are more or equivalent number of 
     * asking shares, else 0
     */
    uint8_t check_bid_shares_ge(uint64_t shares) const;

    /**
     * @brief checks bidding side to see if a given amount 
     * of shares are available as the limit price or better
     * 
     * @param shares the number of shares to check
     * @param limit the limit price to check 
     * @return 1 if there are more or equivalent number of 
     * asking shares, else 0
     */
    uint8_t check_bid_shares_limit_ge(uint64_t shares, uint64_t limit) const;

    /**
     * @brief checks asking side to see if a given amount 
     * of shares are available as the limit price or better
     * 
     * @param shares the number of shares to check
     * @param limit the limit price to check 
     * @return 1 if there are more or equivalent number of 
     * asking shares, else 0
     */
    uint8_t check_ask_shares_limit_ge(uint64_t shares, uint64_t limit) const; 

private:
    std::map<uint64_t, PriceLevel, std::greater<uint64_t>> bids_; 
    std::map<uint64_t, PriceLevel> asks_;
    std::map<uint64_t, Order*> orders_;

    std::map<uint64_t, Message<Order>*> order_map_;
    std::string ticker_; // orderbook identifier    
};

}