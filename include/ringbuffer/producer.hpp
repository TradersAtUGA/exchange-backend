#pragma once

#include <iostream>
#include <atomic>
#include <cstdint>
#include <thread>
#include <chrono>
#include <optional>

#include "shared_memory.hpp"
#include "shared/logger.hpp"
#include "shared/enums.hpp"

template <typename T>
class Producer {
private:
    struct RingBuffer {
        static constexpr uint32_t MAGIC = 0xDEADBEEF;
        std::atomic<uint32_t> magic{0};
        std::atomic<uint64_t> head{0};
        std::atomic<uint64_t> tail{0};
        uint32_t capacity;
    };

    int shared_memory_id;
    std::optional<void*> shared_memory_base;
    RingBuffer* ring;
    T* data;

public:
    Producer(const char* filename, uint32_t capacity, IPCKey proj_id) {
        // integer overflow if capacity is max 
        uint64_t total_capacity = sizeof(RingBuffer) + capacity;

        // create unique key
        key_t key{ftok(filename, static_cast<int>(proj_id))};
        if (key == -1) {
            Logger::error("Error: Could not create key from file");
            throw std::runtime_error("Error: Could not create key from file");
        }

        // create shared memory 
        shared_memory_base = attach_memory_block(key, total_capacity);

        if (!shared_memory_base) {
            throw std::runtime_error("Error: Could not attach to the block");
        }

        // convert raw bytes to ringbuffer
        ring = reinterpret_cast<RingBuffer*>(shared_memory_base);
        data = reinterpret_cast<T*>(shared_memory_base) + sizeof(RingBuffer);

        ring->capacity = capacity;
        ring->head.store(0, std::memory_order_relaxed);
        ring->tail.store(0, std::memory_order_relaxed);

        // signal to other consumers the ring buffer is ready
        ring->magic.store(RingBuffer::MAGIC, std::memory_order_release);

        Logger::info("Producer: Created shared memory and initialized ring buffer");
    }

    bool enqueue(T& item) {
        Logger::info("Enqueueing Item...");

        size_t tail = ring->tail.load(std::memory_order_relaxed);
        size_t next_tail = (tail + 1) % ring->capacity;

        if (next_tail == ring->head.load(std::memory_order_acquire)) {
            return false; // queue full
        }
        
        data[tail] = item;
        ring->tail.store(next_tail, std::memory_order_release);
        return true;
    }
    
    size_t size_approx() const {
        size_t head = ring->head.load(std::memory_order_acquire);
        size_t tail = ring->tail.load(std::memory_order_acquire);

        if (tail >= head) {
            return tail - head;
        } else {
            return ring->capacity - head + tail;
        }
    }

    bool empty() const {
        return size_approx() == 0;
    }

    bool full() const {
        return size_approx() >= ring->capacity - 1;
    }

    ~Producer() {
        detach_memory_block(shared_memory_base); 

        if (remove_memory_block(filename)) {
            Logger::info("Successfully removed memory block");
        } else {
            Logger::error("ERROR: failed to remove_memory_block");
        }
    }
};
