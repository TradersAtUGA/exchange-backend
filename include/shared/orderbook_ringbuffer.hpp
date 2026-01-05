#pragma once 

#include <cstdint>
#include <utility>
#include <ostream>
#include <array>

using std::uint64_t, std::uint8_t;

/*
    NOTE: this implementation does not return errors
    when dequeueing or peeking from an empty ring buffer
    it is up to caller to properly check if the buffer
    contains any elements or is not empty before calling 
    those mentioned methods. 
*/
template <typename T, uint64_t SIZE>
class OrderbookRingBuffer {
public:

    RingBuffer() noexcept : head_(0), tail_(0), item_count_(0) {}

    /**
     * @brief enqueues an item into the ring buffer
     * 
     * @param item the item to enqueue
     */
    void enqueue(T&& item) {
        if (item_count_ + 1 > SIZE) return;
        buf_[tail_] = std::move(item);
        tail_ = (tail_ + 1) % SIZE;
        ++item_count_;
    }

    /** @brief dequeues the earliest item inserted (FIFO) */
    void dequeue() noexcept {
        if (empty()) return;
        head_ = (head_ + 1) % SIZE;
        --item_count_;
    }

    /**
     * @brief view first item in the ring buffer
     * 
     * @return the first item the ring buffer, even if 
     * the ring buffer is empty
     */
    [[nodiscard]] const T& peek() const noexcept { return buf_[head_]; }
    
    [[nodiscard]] T& peek() noexcept { return buf_[head_]; }
 
    /** 
     * @brief returns if the ring buffer contains items or not 
     * 
     * @return 1 if there are no items, else 0 
     */
    uint8_t empty() const noexcept { return (item_count_ == 0); }

    /**
     * @brief returns if the ring buffer is full
     * 
     * @return 1 if the ring buffer is full, else 0 
     */
    uint8_t full() const noexcept { return (item_count_ == SIZE); }

    /**
     * @brief returns the capacity of the buffer, the same
     * value used to instantiate the ring buffer
     * 
     * @return underlying capacity of the ring buffer
     */
    uint64_t capacity() const noexcept { return SIZE; }

    /**
     * @brief returns the number of items in the ring buffer
     * 
     * @return number of current items in the ring buffer
     */
    uint64_t item_count() const noexcept { return item_count_; }

    /** @brief clears all items inside */
    void clear() noexcept {
        head_ = 0;
        tail_ = 0;
        item_count_ = 0;
    }

    /**
     * @brief returns a pointer to beginning of the underlying array.
     * Note that the underlying array may contain data that was previously 
     * dequeued from the ring buffer.
     */
    T* begin() { return &buf_[0]; }
    const T* begin() const { return &buf_[0]; }
    
    /**
     * @brief returns a pointer to the end of the underlying array.
     * Note that the underlying array may contain data that was previously
     * dequeued from the ring buffer
     */
    T* end() noexcept { return &buf_[SIZE]; }
    const T* end() const noexcept { return &buf_[SIZE]; }

    /** @brief returns a pointer to the last item in the ring buffer */
    T* back_ptr() noexcept { return &buf_[(tail_ + SIZE - 1) % SIZE]; } // add SIZE to prevent integer underflow

    /** @brief returns a const pointer to the last item in the ring buffer */
    const T* back_ptr() const noexcept {return buf_[(tail_ + SIZE - 1) % SIZE];}

    /** @brief returns a pointer to the first item in the ring buffer */
    T* front_ptr() noexcept { return &buf_[(head_ + SIZE - 1) % SIZE]; }

    /** @brief returns a const pointer to the first item in the ring buffer */
    const T* front_ptr() const noexcept { return &buf_[(head_ + SIZE - 1) % SIZE]; }

    friend std::ostream& operator<<(std::ostream& os, const RingBuffer& rb) {
        for(uint64_t i{}; i < rb.item_count(); ++i) {
            os << rb.buf_[(rb.head_ + i) % SIZE] << " ";
        }
        return os;
    }

private:
    std::array<T, SIZE> buf_;
    uint64_t head_;
    uint64_t tail_;
    uint64_t item_count_;
};