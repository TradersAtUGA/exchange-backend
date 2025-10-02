#pragma once
#include <string>
#include "shared/order.hpp"

namespace exchange {


class MatchingEngine {
public:
    // Template for matching engine later
    MatchingEngine() = default;

    void submit(exchange::Order&& order);

private:
    std::string ticker_;
    // ringbuffer
    // order book
};


}
