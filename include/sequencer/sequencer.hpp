#pragma once

#include "moodycamel/concurrentqueue.h"
#include "shared/order.hpp"
#include <cstdint>
#include "shared/ring_buffer.hpp"
#include "config.hpp"
#include <atomic>
#include <unordered_map>

namespace exchange {

class Sequencer {
public:
    Sequencer(const std::atomic<bool>& running,
        moodycamel::ConcurrentQueue<exchange::Order>& inbound, 
        std::unordered_map<std::string, std::unique_ptr<RingBuffer<Order, config::RING_BUFFER_SIZE>>>& outbound);

    void operator()();

private:
    // Running Flag - read only
    const std::atomic<bool>& running_;
    
    uint64_t counter_;
    moodycamel::ConcurrentQueue<exchange::Order>& inbound_;
    std::unordered_map<std::string, std::unique_ptr<RingBuffer<Order, config::RING_BUFFER_SIZE>>>& outbound_; //map from ticker to ringbuffers
};

}