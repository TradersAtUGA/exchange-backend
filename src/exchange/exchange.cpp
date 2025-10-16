/**
 * @file exchange.cpp
 * @author Brennan Davenport
 * @date 2025-10-06
 * @brief This exchange object is the controller of the entire program
 */

#include <thread>
#include <chrono>
#include <iostream>
#include <memory> 

#include "exchange/exchange.hpp"
#include "sequencer/sequencer.hpp"

#include "ticker.hpp"


namespace exchange
{
    
Exchange::Exchange() {

}

Exchange::~Exchange() {

}

bool Exchange::init() {
    std::cout << "Exchange is beginning initiation" << std::endl;

    for (const auto& ticker : ticker::TICKERS) {
        sequencer_to_matcher_[ticker] = std::make_unique<RingBuffer<Order, config::RING_BUFFER_SIZE>>();

        matching_engines_.push_back(std::make_unique<MatchingEngine>(ticker, *sequencer_to_matcher_[ticker]))
    }

    sequencer_ = std::make_unique<Sequencer>(network_to_sequencer_, sequencer_to_matcher_);

    std::cout << "Exchange has completed initialization" << std::endl;
    return true;
}

void Exchange::run(std::atomic<bool>& running) {
    // do something with running later
    while(running) {
        std::cout << "Exchange is running" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    // std::thread sequence_worker(*sequencer_);

    // sequence_worker.join();

    std::cout << "Exchange is no longer running" << std::endl;
    return;
}


} // namespace exchange
