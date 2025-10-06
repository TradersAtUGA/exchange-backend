#pragma once

#include "moodycamel/concurrentqueue.h"
#include "order.hpp"
#include <cstdint>
#include "shared/ring_buffer.hpp"
#include "config.hpp"
#include <atomic>

namespace exchange {

class Sequencer {
public:
    Sequencer(moodycamel::ConcurrentQueue<exchange::Order>& inbound, 
        std::unordered_map<std::string, std::unique_ptr<RingBuffer<Order, config::RING_BUFFER_SIZE>>>& outbound);

    void operator()();

private:
    uint64_t counter_;
    moodycamel::ConcurrentQueue<exchange::Order>& inbound_;
    std::unordered_map<std::string, std::unique_ptr<RingBuffer<Order, config::RING_BUFFER_SIZE>>>& outbound_; //map from ticker to ringbuffers
};

}