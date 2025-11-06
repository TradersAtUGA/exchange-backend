/**
 * @brief Orderbook data structure
 */
#pragma once
#include <map>
#include <deque>
#include <cstdint>
#include <optional>
#include <vector>

#include "shared/order.hpp"

namespace exchange {

class OrderBook {
public:

    /**
     * @brief adds an order to the OrderBook
     * 
     * @param order the order being added
     */
    void push_order(exchange::Order&& order) { // Maybe change this to return bool to ensure it can be checked? Else this can just be void 
        orderbook_[order.price].push_back(std::move(order));
    }

    /**
     * @brief removes the first pushed order from a given price level 
     * 
     * @param price_level the price level to remove the order from 
     * @return 1 if the order was successfully removed from the order, else 0 if there 
     * was no orders at that price level
     */
    std::optional<exchange::Order> pop_order(uint64_t price_level) { 
        if (orderbook_[price_level].size() > 0) {
            exchange::Order order = orderbook_[price_level].front();
            orderbook_[price_level].pop_front(); 
            return order;
        } else {
            return std::nullopt;
        }
    }

    /**
     * @brief removes a given order from the OrderBook entirely
     * 
     * @param order the order to remove
     * @return 1 if the order was able to be removed, else 0
     */
    std::optional<exchange::Order> remove_order(const exchange::Order& order) { // This method O(N) because we using std::deque
        u_int64_t count = 0;
        u_int64_t deque_size = (u_int64_t) orderbook_[order.price].size();
        for (const exchange::Order& check : orderbook_[order.price]) {
            if (check.order_id == order.order_id) { 
                auto ele = orderbook_[order.price].begin() + count;
                orderbook_[order.price].erase(ele);
            }
            ++count;
        }

        if (count == deque_size) {
            return std::nullopt;
        }

        return order; 
    }

    /**
     * @brief modifies a given order in the OrderBook 
     * 
     * @param order the order to modify
     */
    void modify_order(exchange::Order&& order) {
        remove_order(order);
        push_order(std::move(order));
    }

private:
    // main order book
    std::map<u_int64_t, std::deque<exchange::Order>> orderbook_;

    // map from client to orders in the order book so removal can be faster
    // we can change the container to be something else instead of just a vector 

    // not being used yet
    std::map<uint64_t, std::vector<exchange::Order>> user_orders_;
};
}

