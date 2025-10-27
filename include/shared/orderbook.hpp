/**
 * @brief Orderbook data structure
 */
#pragma once
#include <map>
#include <deque>
#include <cstdint>

#include "shared/order.hpp"

namespace exchange {

class OrderBook {
public:

    /**
     * @brief Adds an order to the OrderBook
     * 
     * @param order the order being added
     * @return 1 if the order was successfully added to the orderbook
     */
    uint8_t push_order(exchange::Order& order) { // Maybe change this to return bool to ensure it can be checked? Else this can just be void 
        this->orderbook_[order.price].push_back(order);
        return 1;
    }

    /**
     * @brief Removes the first pushed order from a given price level 
     * 
     * @param price_level the price level to remove the order from 
     * @return 1 if the order was successfully removed from the order, else 0 if there 
     * was no orders at that price level
     */
    exchange::Order pop_order(uint64_t price_level) { 
        if (this->orderbook_[price_level].size() > 0) {
            exchange::Order order = this->orderbook_[price_level].front();
            this->orderbook_[price_level].pop_front(); 
            return order;
        } else {

        }
    }

    /**
     * @brief Removes a given order from the orderbook entirely
     * 
     * @param order the order to remove
     * @return 1 if the order was able to be removed, else 0
     */
    uint8_t remove_order(exchange::Order& order) {

        // for (or : this->orderbook_[order.price]) {

        // }

    }

private:

    std::map<u_int64_t, std::deque<exchange::Order>> orderbook_;

};
}

