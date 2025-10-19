#pragma once
#include <string>
#include "shared/order.hpp"
#include "shared/ring_buffer.hpp"
#include "config.hpp"

namespace exchange {

class MatchingEngine {
public:
    // Template for matching engine later
    MatchingEngine(const std::string& ticker, RingBuffer<Order, config::RING_BUFFER_SIZE>& inbound);


private:
    const std::string& ticker_;
    RingBuffer<Order, config::RING_BUFFER_SIZE>& inbound_;
    // order book
};

}
