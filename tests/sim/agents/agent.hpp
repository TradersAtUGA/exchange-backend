#pragma once

#include "matching-engine/matching_engine.hpp"
#include "shared/message.hpp"
#include <vector>
#include <cstdint>

namespace sim {

class Agent {
public:
    explicit Agent(uint64_t client_id)
        : cid_(client_id), next_oid_(client_id * 1'000'000) {}
    virtual ~Agent() = default;

    // Returns orders to submit this tick
    virtual std::vector<Message<exchange::Order>> act(
        const exchange::OrderBook& book,
        uint64_t tick
    ) = 0;

    // Returns cancels to submit this tick
    virtual std::vector<Message<exchange::Order>> cancels(
        const exchange::OrderBook& book,
        uint64_t tick
    ) {
        (void)book; (void)tick;
        return {};
    }

    uint64_t cid() const { return cid_; }

protected:
    uint64_t next_oid() { return next_oid_++; }
    uint64_t cid_;
    uint64_t next_oid_;
};

} // namespace sim
