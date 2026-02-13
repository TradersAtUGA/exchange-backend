    #include "../../pch.h"

#include "matching-engine/orderbook.hpp"

#include <algorithm>
#include <ostream>

using std::uint64_t, std::uint8_t; 


void exchange::OrderBook::add_order(Message<exchange::Order> msg) { 
    if (msg.payload.side == Side::BUY) { 
        bids_[msg.payload.price].push_back(std::forward<Message<exchange::Order>>(msg));
        order_map_[msg.payload.oid] = bids_[msg.payload.price].back_ptr();
    } else { 
        asks_[msg.payload.price].push_back(std::forward<Message<exchange::Order>>(msg));
        order_map_[msg.payload.oid] = asks_[msg.payload.price].back_ptr();
    }
}

void exchange::OrderBook::cancel_order(const Message<Cancel>& msg) { 
    auto order_it = order_map_.find(msg.payload.order_id);
    if (order_it == order_map_.end()) return; // order DNE 
    if (order_it->second->payload.status == Status::CANCELED) return; // order dead

    order_it->second->payload.status = Status::CANCELED; 
    if (order_it->second->payload.side == Side::BUY) { 
        bids_.at(order_it->second->payload.price)
            .reduce_shares(order_it->second->payload.qty);
    } else { 
        asks_.at(order_it->second->payload.price)
            .reduce_shares(order_it->second->payload.qty);
    }
    order_map_.erase(order_it);
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
        if (k == 0) continue; // sentinel order
        os << "For Price Level: " << k << "\n";
        os << "\n";
        os << v << "\n";
        os << "\n";
    }  

    os << "ASKS" << "\n";
    os << "\n";

    for (const auto& [k, v] : ob.asks()) {
        if (k == std::numeric_limits<uint64_t>::max()) continue; // sentinel order
        os << "Price Level: " << k << "\n";
        os << "\n";
        os << v << "\n";
    }
    return os;
}
