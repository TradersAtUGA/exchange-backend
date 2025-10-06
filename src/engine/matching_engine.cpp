#include "engine/matching_engine.hpp"
#include "shared/ring_buffer.hpp"
#include "config.hpp"
#include <string>

namespace exchange {

// add outbound queue later
MatchingEngine::MatchingEngine(const std::string& ticker, RingBuffer<Order, config::RING_BUFFER_SIZE>& inbound) 
     : ticker_(ticker), inbound_(inbound) {}

}