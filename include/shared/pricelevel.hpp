#pragma once

#include <cstdint>
#include <utility>
#include <ostream>

#include "shared/orderbook_ringbuffer.hpp"
#include "shared/order.hpp"
#include "config.hpp"

using std::uint64_t, std::uint8_t;

class PriceLevel {
public:

    PriceLevel() : rb_(), qty_count_(0) {}

    /**
     * @brief pushes an order to the back of the buffer
     * 
     * @param order the order to push back 
     */
    void push_back(exchange::Order&& order) { 
        qty_count_ += order.qty;
        rb_.enqueue(std::move(order)); 
    }

    /** @brief removes the first order  */
    void pop_front() noexcept { 
        qty_count_ -= front().qty;
        rb_.dequeue(); 
    }

    /**
     * @brief removes the first order and reduces 
     * share count by the number of filled orders
     * 
     * @param filled the number of filled orders
     */
    void consume_front(uint64_t filled) noexcept {
        qty_count_ -= filled;
        rb_.dequeue();
    }

    /** 
     * @brief reduces the number of total shares 
     * 
     * @param shares the number of shares to remove
     * from this price level
     */
    void reduce_shares(uint64_t shares) noexcept {
        if (shares > qty_count_) qty_count_ = 0;
        qty_count_ -= shares;
    }


    /** 
     * @brief returns the first order 
     * 
     * @return the first order at this price level
     * */
    [[nodiscard]] exchange::Order& front() noexcept { return rb_.peek(); } 
    [[nodiscard]] const exchange::Order& front() const noexcept { return rb_.peek(); }

    exchange::Order* back_ptr() noexcept { return rb_.back_ptr(); }

    /** @brief returns the total number of orders */
    uint64_t order_count() const noexcept { return rb_.item_count(); }


    /** @brief returns the total number of shares */
    uint64_t total_shares() const noexcept { return qty_count_; }

    /** 
     * @brief check if there are no orders
     * 
     * @return 1 if there are no orders, else 0 
     */
    uint8_t empty() const noexcept { return rb_.empty(); }

    /** 
     * @brief check if no more orders can be pushed back 
     * 
     * @return 1 if no more orders can be added, else 0
     */
    uint8_t full() const noexcept { return rb_.full(); }

    friend std::ostream& operator<<(std::ostream& os, const PriceLevel& pl) {
        os << "Total number of shares: " << pl.qty_count_ << "\n";
        os << pl.rb_ << "\n"; 
        return os;
    }

private:
    OrderbookRingBuffer<exchange::Order, config::RING_BUFFER_SIZE> rb_;
    uint64_t qty_count_;
    
};