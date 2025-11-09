#pragma once
#include <cstdint>
#include <cstddef>
#include <utility>
#include <atomic>

namespace exchange {

template<typename T, size_t Size, size_t NumConsumers>
class MultiConsumerRingBuffer {
public:
    // To - do implement the rest of this
    MultiConsumerRingBuffer() {}

    bool enqueue(T&& item) {}

    bool dequeue(T& item) {}

    bool empty() const {}

    bool full() const {}

    int size() const {}

private:
    T buffer_[Size];
    std::atomic<size_t> tail_;
    std::array<std::atomic<size_t>, NumConsumers> heads_; 
};

}
