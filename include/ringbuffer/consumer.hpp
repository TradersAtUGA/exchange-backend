/**
 * @file consumer.hpp
 * @author Brennan Davenport
 * @date 2026-2-20
 * @brief Lock-free SPSC ring buffer consumer over SysV shared memory.
 *
 * Provides the read-side of a single-producer/single-consumer queue where
 * both endpoints live in separate processes. Coordination is achieved entirely
 * through a pair of atomic cursors (head/tail) and carefully chosen memory
 * orderings — no mutex or semaphore is involved, keeping the dequeue hot-path
 * as cheap as a cache-line load and a modulo. The underlying shared memory
 * segment is created and owned by the producer; this class only attaches to
 * and detaches from it.
 */
#pragma once

#include <iostream>
#include <atomic>
#include <cstdint>
#include <thread>
#include <chrono>

#include "shared_memory.hpp"
#include "shared/logger.hpp"
#include "shared/enums.hpp"

template <typename T>
class Consumer {
    static_assert(std::is_trivially_copyable_v<T>,
        "Consumer<T> requires a trivially copyable type T is copied "
        "raw across a shared memory boundary with no constructor/destructor invocation.");
private:
    /**
     * @brief Ring buffer control block that lives at the front of the shared memory segment.
     *
     * By placing this struct at the very start of the shared memory region, both the producer
     * and consumer can locate it with a simple reinterpret_cast of the base pointer, no offset
     * negotiation required. All coordination fields are atomic so cross-process reads/writes
     * are safe without a mutex, keeping the fast path lock-free.
     */
    struct RingBuffer {
        /**
         * @brief Sentinel value written by the producer during initialization.
         *
         * 0xDEADBEEF is a well-known magic number that is highly unlikely to appear
         * in uninitialized memory by coincidence. The consumer checks for this value
         * before use to detect the race where it attaches before the producer has
         * finished setting up the segment, preventing reads from garbage data.
         */
        static constexpr uint32_t MAGIC = 0xDEADBEEF;

        /// @brief Written once by the producer; read by the consumer to confirm the segment is ready.
        std::atomic<uint32_t> magic;

        /**
         * @brief Read cursor, owned exclusively by the consumer.
         *
         * Only the consumer ever advances head, so no compare-exchange is needed —
         * a plain store after the data read is sufficient. The 64-bit width avoids
         * wrap-around even at millions of messages per second for years of uptime,
         * so the modulo against capacity is the only wrap logic required.
         */
        std::atomic<uint64_t> head;

        /**
         * @brief Write cursor, owned exclusively by the producer.
         *
         * The consumer only reads tail to check emptiness; the producer is the sole
         * writer. This single-writer guarantee is what makes the SPSC design
         * lock-free: no two threads ever race to advance the same cursor.
         */
        std::atomic<uint64_t> tail;

        /// @brief Fixed slot count established at construction; never changes after init.
        uint32_t capacity;
    };


    /**
     * @brief Base address of the attached shared memory segment, wrapped in optional.
     *
     * Using optional lets detach_memory_block distinguish "never attached" from a
     * valid (but potentially null-valued) pointer, avoiding a double-detach on
     * destruction if the constructor throws partway through.
     */
    std::optional<void*> shared_memory_base;

    /// @brief Pointer into shared_memory_base; addresses the control block directly.
    RingBuffer* ring;

    /**
     * @brief Pointer to the first data slot, positioned immediately after the control block.
     *
     * Computed once at attach time so every dequeue is a direct indexed array access
     * rather than a repeated base + offset calculation.
     */
    T* data;

public:
    /**
     * @brief Attaches to an existing shared memory ring buffer created by the producer.
     *
     * The constructor intentionally throws rather than leaving the object in a
     * half-initialised state — callers can rely on a successfully constructed
     * Consumer being immediately usable without a separate "is_valid()" check.
     *
     * @param filename Path to an existing file used as the ftok seed. Must match
     *                 the path the producer used so both sides derive the same key.
     * @param capacity Number of slots the producer allocated. Must match exactly;
     *                 a mismatch would make the data pointer land in the wrong place.
     * @param proj_id  Project identifier passed to ftok. Combined with filename it
     *                 produces a unique IPC key, allowing multiple independent queues
     *                 to coexist on the same host without colliding.
     * @throws std::runtime_error if the IPC key cannot be derived, the shared memory
     *         segment cannot be attached, or the magic number check fails.
     */
    Consumer(const char* filename, uint32_t capacity, IPCKey proj_id) {
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

        // convert bytes into this class
        ring = reinterpret_cast<RingBuffer*>(shared_memory_base);
        data = reinterpret_cast<T*>(shared_memory_base) + sizeof(RingBuffer);

        // validate initialization
        if (ring->magic.load(std::memory_order_acquire) != RingBuffer::MAGIC) {
            throw std::runtime_error("Shared memory not initialized yet");
        }

        Logger::info("Consumer: Attached to shared memory")
    }

    /**
     * @brief Attempts to pop one item from the ring buffer without blocking.
     *
     * The memory ordering pair — relaxed load of head, acquire load of tail — is
     * deliberate. Head is private to the consumer so no synchronisation on that load
     * is needed. Tail is written by the producer; acquire ensures that any stores the
     * producer made to the data slot before advancing tail are visible here before we
     * read from data[head]. The release store of the updated head then publishes the
     * freed slot back to the producer.
     *
     * @param[out] item Populated with the dequeued value when the call succeeds.
     * @return true  if an item was available and item has been written.
     * @return false if the queue was empty at the time of the call; item is unchanged.
     */
    bool dequeue(T& item) {
        size_t head = ring->head.load(std::memory_order_relaxed);

        if (head == ring->tail.load(std::memory_order_acquire)) {
            return false; // queue empty
        }

        // MUST COPY DATA
        item = data[head];
        ring->head.store((head + 1) % ring->capacity, std::memory_order_release);
        return true;
    }

    /**
     * @brief Returns an approximate number of items currently in the queue.
     *
     * Both cursors are sampled independently with no global lock, so the result
     * may be stale by the time the caller inspects it — hence "approx". The
     * acquire ordering on both loads ensures we see a consistent snapshot of each
     * cursor relative to its own writer, but there is no guarantee the two loads
     * are atomic with respect to each other.
     *
     * The tail < head branch handles the single case where the write cursor has
     * wrapped around the ring while the read cursor has not yet caught up.
     *
     * @return Approximate number of unconsumed slots.
     */
    size_t size_approx() const {
        size_t head = ring->head.load(std::memory_order_acquire);
        size_t tail = ring->tail.load(std::memory_order_acquire);

        if (tail >= head) {
            return tail - head;
        } else {
            return ring->capacity - head + tail;
        }
    }

    /**
     * @brief Convenience predicate; true when no items are available to dequeue.
     *
     * Delegates to size_approx(), so the same "may be stale" caveat applies —
     * a true result does not guarantee that a subsequent dequeue() will fail if
     * the producer is concurrently writing.
     *
     * @return true if the queue appears empty.
     */
    bool empty() const {
        return size_approx() == 0;
    }

    /**
     * @brief Convenience predicate; true when no free slots remain for the producer.
     *
     * The capacity - 1 threshold is intentional: the ring buffer sacrifices one
     * slot to distinguish "full" (head == tail + 1) from "empty" (head == tail)
     * without needing a separate counter, which would require additional
     * synchronisation.
     *
     * @return true if the queue appears full.
     */
    bool full() const {
        return size_approx() >= ring->capacity - 1;
    }

    /**
     * @brief Detaches the shared memory segment on destruction.
     *
     * Detaching decrements the kernel's attach count but does not destroy the
     * segment — that is the producer's responsibility. This asymmetry is correct:
     * the consumer should never unilaterally delete memory the producer may still
     * be writing to.
     */
    ~Consumer() {
        detach_memory_block(shared_memory_base);
    }
};