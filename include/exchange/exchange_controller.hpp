/**
 * @file exchange_controller.hpp
 * @author Brennan Davenport
 * @date 2025-10-06
 * @brief This exchange interface is the controller 
 * of the entire exchange. The role of this controller is
 * to control and direct managers (network, thread, etc.), 
 * additionally, the controller should own all the infrastructure
 * for how the managers communicate (queues, messages)
 */
#pragma once


// #include "shared/order.hpp"
#include "shared/ring_buffer.hpp"

// forward decleration
class Sequencer; // forward declaratoin better than include do for all
// #include "sequencer/sequencer.hpp"
#include "engine/matching_engine.hpp"
#include "network-manager/network_manager.hpp"

// #include "config.hpp"
// #include "ticker.hpp"


namespace exchange
{

class ExchangeController {
public:
    /**
     * @brief paramerterized constructor to create a new ExchangeController instance
     * @param running is the current way that the user can signal to the Controller to stop
     */
    ExchangeController(const std::atomic<bool>& running);
    /**
     * @brief default deconstructor
     */
    ~ExchangeController() = default;

    /**
     * @brief Initializes sub tasks
     * - Creates all tickers
     * - Creates all matching engines
     * - Creates sequencer
     * - Creates queues between componenets
     * @return a bool to indicate if successfully initialized componenets
     */
    bool init();

    /**
     * @brief Starts all the components of the exchange and
     * begins accepting, matching, and sending out orders, and transactions
     */
    void run();

    /**
     * @brief Shuts down exchange, joins threads and saves data
     */
    void shutdown();

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
