#pragma once
#include <cstdint>
#include <cstddef>
#include <utility>
#include <semaphore>
#include <atomic>

namespace exchange {

template<typename T, size_t Capacity>
class RingBuffer {
    static_assert((Capacity & (Capacity - 1)) == 0, "RingBuffer Capacity must be power of 2");
public:
    RingBuffer() = default;

    bool enqueue(T&& item) {
        size_t current_tail = tail_.load(std::memory_order_relaxed);
        size_t next_tail = (current_tail + 1) & (Capacity - 1);

        if (next_tail == head_.load(std::memory_order_acquire)) {
            return false; // queue full
        }
        
        buffer_[current_tail] = std::move(item);
        tail_.store(next_tail, std::memory_order_release);
        count_.fetch_add(1, std::memory_order_relaxed);
        return true;
    }

    bool dequeue(T& item) {
        size_t current_head = head_.load(std::memory_order_acquire);
        size_t next_head = (current_head + 1) & (Capacity - 1);

        if (current_head == tail_.load(std::memory_order_relaxed)) {
            return false; // queue empty
        }

        item = std::move(buffer_[current_head]);
        head_.store(next_head, std::memory_order_release);
        count_.fetch_sub(1, std::memory_order_relaxed);
        return true;
    }

    int size() const {
        return count_.load(std::memory_order_relaxed);
    }

private:
    T buffer_[Capacity];
    std::atomic<size_t> head_;
    std::atomic<size_t> tail_;
    std::atomic<size_t> count_;
};

}
