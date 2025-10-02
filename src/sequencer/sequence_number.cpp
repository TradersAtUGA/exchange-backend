#include "sequencer/sequence_number.hpp"
#include <cstdint>
#include <atomic>
#include <memory>

namespace exchange {

SequenceNumber::SequenceNumber(uint64_t start) {
    counter_ = start;
}

uint64_t SequenceNumber::next() {
    return counter_.fetch_add(1, std::memory_order_relaxed);
}

uint64_t SequenceNumber::current() const {
    return counter_.load(std::memory_order_relaxed);
}

void SequenceNumber::reset(uint64_t start) {
    counter_.store(start, std::memory_order_relaxed);
}

std::unique_ptr<SequenceNumber> create_sequence_number() {
    return std::make_unique<SequenceNumber>();
}

}