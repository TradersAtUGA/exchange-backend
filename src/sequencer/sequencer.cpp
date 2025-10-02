#include "sequencer/sequencer.hpp"
#include "sequencer/sequence_number.hpp"
#include <iostream>

namespace exchange {


void Sequencer::submit(exchange::Order&& order) {
    // Accepts an rvalue and pushes it into the inbound queue
    inbound_.enqueue(std::move(order));
}

void Sequencer::run() {
    auto sequence_ptr = create_sequence_number();

    while(true) {
        exchange::Order order;
        if (inbound_.try_dequeue(order)) {
            std::cout << "Got order: " << order << std::endl;

            // Handle logic 
            /* 
            Give timestamp that is global monotonic
            Push order onto correct Ring Buffer for respective matching engine
            */
            
            order.sequence_id = sequence_ptr->next();
            
            
        } else {
            // Maybe sleep thread for a little to save CPU usage 
        }

        
    }
}


}