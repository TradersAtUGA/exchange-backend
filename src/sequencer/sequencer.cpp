#include "sequencer/sequencer.hpp"
#include <iostream>
#include <unordered_map>

namespace exchange {

Sequencer::Sequencer(moodycamel::ConcurrentQueue<exchange::Order>& inbound, 
        std::unordered_map<std::string, std::unique_ptr<RingBuffer<Order, config::RING_BUFFER_SIZE>>>& outbound)
         : inbound_(inbound), outbound_(outbound), counter_(0) {}


// opperator override for std::thread
void Sequencer::operator()() {
    while(true) { // maybe change later
        exchange::Order order;
        if (inbound_.try_dequeue(order)) {
            std::cout << "Got order: " << order.order_id << std::endl; // cannot just print the "order" object itself 

            // Handle logic 
            /* 
            Give timestamp that is global monotonic
            Push order onto correct Ring Buffer for respective matching engine
            */
            order.sequence_id = counter_++;
            
            // Route ticker, assume ticker exsits
            while(!outbound_[order.ticker]->enqueue(std::move(order))) {
                std::this_thread::yield();
            }

        } else {
            // Maybe sleep thread for a little to save CPU usage 
            std::this_thread::yield();
        }
    }
}

}