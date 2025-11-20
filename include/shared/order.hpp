#pragma once
#include <cstdint>
#include <string>


enum struct Side : uint8_t {
    Buy = 0,
    Sell = 1
};

enum struct OrderType : uint8_t {
    Limit = 0,
    Market = 1
};

namespace exchange {
    // rule of 0 since Order has no RAII then compiler defines all 5 constructors
    struct Order {
        // Sequence
        uint64_t sequence_id;

        // Client details
        uint64_t client_id;
        uint64_t order_id;

        // Order details
        Side side;
        OrderType type;
        std::string ticker;
        uint64_t price;      // Price is never negative
        uint64_t quantity;   // Quantity is never negative 
        uint64_t remaining;  // Partial fill

        // Timing
        uint64_t received_time_ns;
    };

}
