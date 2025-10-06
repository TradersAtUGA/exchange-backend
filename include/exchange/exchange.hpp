#pragma once
#include "moodycamel/concurrentqueue.h"
#include "order.hpp"
#include "config.hpp"
#include "shared/ring_buffer.hpp"
#include <unordered_map>
#include "include/sequencer/sequencer.hpp"
#include "include/engine/matching_engine.hpp"
#include <vector>
#include <atomic>

namespace exchange
{

class Exchange {
public:
    Exchange();
    ~Exchange();

    bool init();
    void run(const std::atomic<bool>& running);

private:
    // Queues
    moodycamel::ConcurrentQueue<exchange::Order> network_to_sequencer_;
    std::unordered_map<std::string, 
        std::unique_ptr<RingBuffer<Order, config::RING_BUFFER_SIZE>>> sequencer_to_matcher_;

    // Components
    std::unique_ptr<Sequencer> sequencer_;
    std::vector<std::unique_ptr<MatchingEngine>> matching_engines_;
    // vikas add ur network-in manager here
};


} 
