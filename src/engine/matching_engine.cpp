#include "engine/matching_engine.hpp"
#include "shared/ring_buffer.hpp"
#include "config.hpp"

namespace exchange {

class MatchingEngine {
public:
    void submit(exchange::Order&& order) {
        
    }
private:
    RingBuffer<Order, config::RING_BUFFER_SIZE> order_queue;
};

}