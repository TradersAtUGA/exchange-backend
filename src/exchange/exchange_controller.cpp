/**
 * @file exchange_controller.cpp
 * @author Brennan Davenport
 * @date 2025-10-06
 * @brief This exchange object is the controller of the entire program
 */

// #include <thread>
// #include <chrono>
// #include <iostream>
// #include <memory> 

#include "pch.hpp"
#include "sequencer/sequencer.hpp"
#include "exchange/exchange_controller.hpp"
#include "engine/matching_engine.hpp"
// #include "shared/debug.hpp"
#include "network-manager/network_manager.hpp"

// #include "ticker.hpp"
// #include "config.hpp"


namespace exchange
{
    
ExchangeController::ExchangeController(const std::atomic<bool>& running) : running_(running) {}

ExchangeController::~ExchangeController() {

}

bool ExchangeController::init() {
    DEBUG_PRINT("Exchange is beginning initiation");

    for (const auto& ticker : exchange::TICKERS) {
        sequencer_to_matcher_[ticker] = std::make_unique<RingBuffer<Order, config::RING_BUFFER_SIZE>>();

        matching_engines_.push_back(std::make_unique<MatchingEngine>(ticker, *sequencer_to_matcher_[ticker]));
    }

    sequencer_ = std::make_unique<Sequencer>(running_, network_to_sequencer_, sequencer_to_matcher_);

    DEBUG_PRINT("Exchange has completed initialization");
    return true;
}

void ExchangeController::run() {
    network_manager_.start_inbound_server(network_to_sequencer_);
    DEBUG_PRINT("Starting Network");
    std::thread sequence_worker(*sequencer_);
    DEBUG_PRINT("Starting Sequencer");
    // do something with running later
    while(running_.load(std::memory_order_acquire)) {
        // debug info
        DEBUG_PRINT("Exchange is running");
        DEBUG_PRINT("Network to sequencer queue size: " + std::to_string(network_to_sequencer_.size_approx()));
        DEBUG_PRINT("Ring Buffer size [QQQ]: " + std::to_string(sequencer_to_matcher_["QQQ"]->size()));
        DEBUG_PRINT("Ring Buffer size [ABC]: " + std::to_string(sequencer_to_matcher_["ABC"]->size()));
        DEBUG_PRINT("Ring Buffer size [XYZ]: " + std::to_string(sequencer_to_matcher_["XYZ"]->size()));
        
        // slow done for mvp
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    DEBUG_PRINT("Shutting down network");
    network_manager_.stop_inbound_server();

    DEBUG_PRINT("Shutting down sequencer");
    sequence_worker.join();
    DEBUG_PRINT("Hello World");
    DEBUG_PRINT("Exchange is no longer running");
    return;
}

void ExchangeController::shutdown() {
    return;
}


} // namespace exchange
