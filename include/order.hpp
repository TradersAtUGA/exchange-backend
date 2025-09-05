#pragma once
#include <cstdint>

enum struct Side : uint8_t {
    Buy = 0,
    Sell = 1
};

enum struct OrderType : uint8_t {
    Limit = 0,
    Market = 1
};

namespace exchange {

    struct Order {
        // Sequence
        uint64_t sequence_id;

        // Client details
        uint64_t client_id;
        uint64_t order_id;

        // Order details
        Side side;
        OrderType type;
        uint64_t price;      // Price is never negative
        uint64_t quantity;   // Quantity is never negative 
        uint64_t remaining;  // Partial fill

        // Timing
        uint64_t received_time_ns;

        Order(
            uint64_t client,
            uint64_t oid,
            Side s,
            OrderType t,
            uint64_t p,
            uint64_t q,
            uint64_t r,
            uint64_t ns
        )
        : sequence_id(0),
          client_id(client),
          order_id(oid),
          side(s),
          type(t),
          price(p),
          quantity(q),
          remaining(r),
          received_time_ns(ns)
        {}
    };

}
