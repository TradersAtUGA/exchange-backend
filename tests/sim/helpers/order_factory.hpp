#pragma once

#include "matching-engine/matching_engine.hpp"
#include "shared/message.hpp"

namespace sim {

// Helper to create a Message<Order> wrapping an Order with minimal boilerplate.
// Uses raw integer prices (not bit-cast doubles) since we test at the matching-engine level.

inline Message<exchange::Order> make_limit(
    Side side, uint64_t price, uint64_t qty,
    uint64_t oid, uint64_t coid = 0,
    const char* ticker = "TEST"
) {
    exchange::Order o = exchange::Order::create_new_order(
        side, OrderType::LIMIT, TIF::DAY,
        const_cast<char*>(ticker), price, qty, oid, coid
    );
    Message<exchange::Order> msg;
    msg.payload = o;
    return msg;
}

inline Message<exchange::Order> make_limit_ioc(
    Side side, uint64_t price, uint64_t qty,
    uint64_t oid, uint64_t coid = 0,
    const char* ticker = "TEST"
) {
    exchange::Order o = exchange::Order::create_new_order(
        side, OrderType::LIMIT, TIF::IOC,
        const_cast<char*>(ticker), price, qty, oid, coid
    );
    Message<exchange::Order> msg;
    msg.payload = o;
    return msg;
}

inline Message<exchange::Order> make_limit_fok(
    Side side, uint64_t price, uint64_t qty,
    uint64_t oid, uint64_t coid = 0,
    const char* ticker = "TEST"
) {
    exchange::Order o = exchange::Order::create_new_order(
        side, OrderType::LIMIT, TIF::FOK,
        const_cast<char*>(ticker), price, qty, oid, coid
    );
    Message<exchange::Order> msg;
    msg.payload = o;
    return msg;
}

inline Message<exchange::Order> make_market(
    Side side, uint64_t qty,
    uint64_t oid, uint64_t coid = 0,
    const char* ticker = "TEST"
) {
    exchange::Order o = exchange::Order::create_new_order(
        side, OrderType::MARKET, TIF::DAY,
        const_cast<char*>(ticker), 0, qty, oid, coid
    );
    Message<exchange::Order> msg;
    msg.payload = o;
    return msg;
}

// Create a cancel message using the Order-based cancel path
inline Message<exchange::Order> make_cancel(
    uint64_t cancel_order_id, uint64_t cancel_id = 0
) {
    exchange::Order o = exchange::Order::create_cancel(cancel_order_id, cancel_id);
    Message<exchange::Order> msg;
    msg.payload = o;
    return msg;
}

} // namespace sim
