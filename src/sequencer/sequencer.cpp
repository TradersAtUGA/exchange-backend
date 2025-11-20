#include "sequencer/sequencer.hpp"
#include <iostream>
#include <unordered_map>
#include "shared/debug.hpp"

namespace exchange {

Sequencer::Sequencer(const std::atomic<bool>& running,
        moodycamel::ConcurrentQueue<exchange::Order>& inbound, 
        std::unordered_map<std::string, std::unique_ptr<RingBuffer<Order, config::RING_BUFFER_SIZE>>>& outbound)
         : running_(running), inbound_(inbound), outbound_(outbound), counter_(0) {}


// opperator override for std::thread
void Sequencer::operator()() {
    while(running_.load(std::memory_order_acquire)) { 
        exchange::Order order;
        if (inbound_.try_dequeue(order)) {
            DEBUG_PRINT("Got order: " + std::to_string(order.order_id));

            // Handle logic 
            /* 
            Give timestamp that is global monotonic
            Push order onto correct Ring Buffer for respective matching engine
            */
            order.sequence_id = counter_++;
            
            // Route ticker, assume ticker exsits
            while(!outbound_[order.ticker]->enqueue(std::move(order)) && (running_.load(std::memory_order_acquire))) {
                std::this_thread::yield();
            }

        } else {
            // Maybe sleep thread for a little to save CPU usage 
            std::this_thread::yield();
        }
    }
}

}