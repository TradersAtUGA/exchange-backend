#pragma once

#include "moodycamel/concurrentqueue.h"
#include "order.hpp"

namespace exchange {

class Sequencer {
public:
    void submit(exchange::Order&& order);
    void run();

private:
    moodycamel::ConcurrentQueue<exchange::Order> inbound_;
};

}