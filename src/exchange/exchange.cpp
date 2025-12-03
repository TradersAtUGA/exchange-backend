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

#include "sequencer/sequencer.hpp"
#include "exchange/exchange.hpp"
#include "engine/matching_engine.hpp"
#include "shared/debug.hpp"
#include "network-manager/network_manager.hpp"

#include "ticker.hpp"
#include "config.hpp"


namespace exchange
{
    
Exchange::Exchange(const std::atomic<bool>& running) : running_(running) {}

Exchange::~Exchange() {

}

bool Exchange::init() {
    DEBUG_PRINT("Exchange is beginning initiation");

    for (const auto& ticker : exchange::TICKERS) {
        sequencer_to_matcher_[ticker] = std::make_unique<RingBuffer<Order, config::RING_BUFFER_SIZE>>();

        matching_engines_.push_back(std::make_unique<MatchingEngine>(ticker, *sequencer_to_matcher_[ticker]));
    }

    sequencer_ = std::make_unique<Sequencer>(running_, network_to_sequencer_, sequencer_to_matcher_);

    DEBUG_PRINT("Exchange has completed initialization");
    return true;
}

void Exchange::run() {
    // network_manager_.start_inbound_server(network_to_sequencer_);
    network_manager_.start_gateway();
    DEBUG_PRINT("Starting Network");
    std::thread sequence_worker(*sequencer_);
    DEBUG_PRINT("Starting Sequencer");
    // do something with running later
    while(running_.load()) {
        DEBUG_PRINT("Exchange is running");
        DEBUG_PRINT(network_to_sequencer_.size_approx());
  
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    DEBUG_PRINT("Shutting down network");
    // network_manager_.stop_inbound_server();

    DEBUG_PRINT("Shutting down sequencer");
    sequence_worker.join();

    DEBUG_PRINT("Exchange is no longer running");
    return;
}


} // namespace exchange
