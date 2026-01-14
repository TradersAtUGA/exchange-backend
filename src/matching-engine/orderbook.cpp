#include "../../pch.h"

#include "matching-engine/orderbook.hpp"

#include <algorithm>
#include <ostream>

using std::uint64_t, std::uint8_t; 

void exchange::OrderBook::add_order(exchange::Order order) {
    if (order.side == Side::BUY) { // convert to branchless later (hashmap with ref to bids/asks)
        // k, v: oid : last item in orderbook at that price in the ring buffer
        orders_[order.oid] = bids_[order.price].back_ptr(); 
        bids_[order.price].push_back(std::forward<exchange::Order>(order)); // putting order into orderbook
    } else {
        orders_[order.oid] = asks_[order.price].back_ptr();
        asks_[order.price].push_back(std::forward<exchange::Order>(order));
    }
}  

void exchange::OrderBook::cancel_order(exchange::Order& order) {
    if (order.status == 0) return; // order already dead or filled TODO(vikas): modify this to account for all possible order types 
    auto order_it = orders_.find(order.oid);
    if (order_it == orders_.end()) return; // order DNE



    order_it->second->status = 0; // set to dead 
    orders_.erase(order_it); // erase from lookup map 

    std::cout << "order stat is" << static_cast<int>(order.status) << "\n";

    if (order.side == Side::BUY) { // decrease qty at price level
        bids_.at(order.price).reduce_shares(order.qty);
    } else {
        asks_.at(order.price).reduce_shares(order.qty);
    }
}

void exchange::OrderBook::remove_order_ptr(uint64_t id) { 
    auto it = orders_.find(id);
    if (it != orders_.end()) orders_.erase(id);
}

uint8_t exchange::OrderBook::check_ask_shares_ge(uint64_t shares) const { 
    auto it = asks_.begin();
    uint64_t counted_shares = 0;
    while (it != asks_.end()) { 
        counted_shares += it->second.total_shares();
        if (counted_shares >= shares) return 1;
        ++it;
    }
    return 0;
}

uint8_t exchange::OrderBook::check_bid_shares_ge(uint64_t shares) const { 
    auto it = bids_.begin();
    uint64_t counted_shares = 0;
    while (it != bids_.end()) { 
        counted_shares += it->second.total_shares();
        if (counted_shares >= shares) return 1;
        ++it;
    }
    return 0;
}

uint8_t exchange::OrderBook::check_ask_shares_limit_ge(uint64_t shares, uint64_t limit) const {
    auto it = asks_.begin();
    uint64_t counted_shares = 0;
    while (it != asks_.end()) {
        if (it->first > limit && counted_shares >= shares) return 1;
        if (it->first > limit && counted_shares < shares) return 0; 

        counted_shares += it->second.total_shares();
        if (counted_shares >= shares) return 1;
        ++it;
    }   
    return 0;
}

uint8_t exchange::OrderBook::check_bid_shares_limit_ge(uint64_t shares, uint64_t limit) const {
    auto it = bids_.begin();
    uint64_t counted_shares = 0;
    while (it != bids_.end()) {
        if (it->first < limit && counted_shares >= shares) return 1;
        if (it->first < limit && counted_shares < shares) return 0; 

        counted_shares += it->second.total_shares();
        if (counted_shares >= shares) return 1;
        ++it;
    }   
    return 0;
}

std::ostream& operator<<(std::ostream& os, const exchange::OrderBook& ob) {
    os << "BIDS" << "\n";
    os << "\n";

    for (const auto& [k, v] : ob.bids()) {
        if (k == 0) continue;
        os << "For Price Level: " << k << "\n";
        os << "\n";
        os << v << "\n";
        os << "\n";
    }  

    os << "ASKS" << "\n";
    os << "\n";

    for (const auto& [k, v] : ob.asks()) {
        if (k == std::numeric_limits<uint64_t>::max()) continue;
        os << "Price Level: " << k << "\n";
        os << "\n";
        os << v << "\n";
    }
    return os;
}
