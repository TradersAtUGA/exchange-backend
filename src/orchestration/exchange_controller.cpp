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

#include "pch.h"

#include "sequencer/sequencer.hpp"
#include "exchange/exchange_controller.hpp"
#include "engine/matching_engine.hpp"
#include "network-manager/network_manager.hpp"

#include "shared/logger.hpp"

#include "ticker.hpp"
#include "config.hpp"


namespace exchange
{
    
ExchangeController::ExchangeController(const std::atomic<bool>& running) : running_(running) {}


bool ExchangeController::init() {
    Logger::info("Exchange is beinging initialized");

    for (const auto& ticker : exchange::TICKERS) {
        sequencer_to_matcher_[ticker] = std::make_unique<RingBuffer<Order, config::RING_BUFFER_SIZE>>();

        matching_engines_.push_back(std::make_unique<MatchingEngine>(ticker, *sequencer_to_matcher_[ticker]));
    }

    sequencer_ = std::make_unique<Sequencer>(running_, network_to_sequencer_, sequencer_to_matcher_);

    Logger::info("Exchange has completed initialization");
    return true;
}

void ExchangeController::run() {
    network_manager_.start_inbound_server(network_to_sequencer_);
    std::thread sequence_worker(*sequencer_);

    // do something with running later
    while(running_.load(std::memory_order_acquire)) {
        // debug info
        Logger::info("Exchange is running");
        // DEBUG_PRINT("Exchange is running");
        // DEBUG_PRINT("Network to sequencer queue size: " + std::to_string(network_to_sequencer_.size_approx()));
        // DEBUG_PRINT("Ring Buffer size [QQQ]: " + std::to_string(sequencer_to_matcher_["QQQ"]->size()));
        // DEBUG_PRINT("Ring Buffer size [ABC]: " + std::to_string(sequencer_to_matcher_["ABC"]->size()));
        // DEBUG_PRINT("Ring Buffer size [XYZ]: " + std::to_string(sequencer_to_matcher_["XYZ"]->size()));
        //
        // slow done for mvp
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    network_manager_.stop_inbound_server();
    sequence_worker.join();
    return;
}

void ExchangeController::shutdown() {
    return;
}


} // namespace exchange
