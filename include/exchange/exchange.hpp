/**
 * @file exchange.hpp
 * @author Brennan Davenport
 * @date 2025-10-06
 * @brief This exchange interface is the controller of the entire program
 */
#pragma once

#include <vector>
#include <atomic>
#include <memory>
#include <unordered_map>

#include "moodycamel/concurrentqueue.h"

#include "shared/order.hpp"
#include "shared/ring_buffer.hpp"
#include "sequencer/sequencer.hpp"
#include "engine/matching_engine.hpp"
#include "network-manager/network_manager.hpp"

#include "config.hpp"
#include "ticker.hpp"


namespace exchange
{

class Exchange {
public:
    Exchange(const std::atomic<bool>& running);
    ~Exchange();

    bool init();
    void run();

private:
    // Running Flag - read only
    const std::atomic<bool>& running_;

    // Queues
    moodycamel::ConcurrentQueue<exchange::Order> network_to_sequencer_;
    std::unordered_map<std::string, 
        std::unique_ptr<RingBuffer<Order, config::RING_BUFFER_SIZE>>> sequencer_to_matcher_;

    // Components
    std::unique_ptr<Sequencer> sequencer_;
    std::vector<std::unique_ptr<MatchingEngine>> matching_engines_;
    NetworkManager network_manager_;
};


} 
