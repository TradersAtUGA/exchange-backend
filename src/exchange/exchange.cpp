#include "include/exchange/exchange.hpp"
#include <thread>
#include "include/sequencer/sequencer.hpp"
#include "ticker.hpp"

namespace exchange
{
    
Exchange::Exchange() {

}

Exchange::~Exchange() {

}

bool Exchange::init() {
    
    for (const auto& ticker : ticker::TICKERS) {
        sequencer_to_matcher_[ticker] = std::make_unique<RingBuffer<Order, config::RING_BUFFER_SIZE>>();

        matching_engines_.push_back(std::make_unique<MatchingEngine>(ticker, *sequencer_to_matcher_[ticker]))
    }

    sequencer_ = std::make_unique<Sequencer>(network_to_sequencer_, sequencer_to_matcher_);

    return true;
}

void Exchange::run(std::atomic<bool>& running) {
    // do something with running later

    std::thread sequence_worker(*sequencer_);

    sequence_worker.join();
    return;
}


} // namespace exchange
