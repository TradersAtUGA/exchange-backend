#include "engine/matching_engine.hpp"
#include "shared/ring_buffer.hpp"
#include "shared/multi_ring_buffer.hpp"
#include "config.hpp"
#include <string>

namespace exchange {

// add outbound queue later
MatchingEngine::MatchingEngine(const std::string& ticker, 
     RingBuffer<Order, config::RING_BUFFER_SIZE>& inbound) : ticker_(ticker), inbound_(inbound) {}

// MatchingEngine::MatchingEngine(const std::string& ticker, 
//      RingBuffer<Order, config::RING_BUFFER_SIZE>& inbound, 
//      MultiConsumerRingBuffer<Order, config::RING_BUFFER_SIZE, config::NUM_CONSUMERS>& outbound,     
//      ) : ticker_(ticker), inbound_(inbound), outbound_(outbound)  {}
}