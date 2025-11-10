#pragma once
#include <cstdint>
#include <cstddef>
#include <utility>
#include <semaphore>

namespace exchange {

template<typename T, size_t Size>
class RingBuffer {
public:
    RingBuffer() {
        head_ = 0;
        tail_ = 0;
    }

    bool enqueue(T&& item) {
        spaces_.acquire(); // wait for space
        mutex_.acquire(); // wait for exclusive access to buffer

        buffer_[tail_] = std::move(item);
        tail_ = (tail_ + 1) % Size;
        
        mutex_.release(); // release exclusive access
        items_.release(); // signal that an item is available
        return true;
    }

    bool dequeue(T& item) {
        items_.acquire(); // wait for an item
        mutex_.acquire(); // wait for exclusive access to buffer

        item = std::move(buffer_[head_]);
        head_ = (head_ + 1) % Size;

        mutex_.release(); // release exclusive access
        spaces_.release(); // signal that space is available
        return true;
    }

    bool empty() {
        return (items_.try_acquire() == false); // if we can't acquire, it's empty
    }

    bool full() {
        return (spaces_.try_acquire() == false); // if we can't acquire, it's full
    }

private:
    T buffer_[Size];
    size_t head_, tail_;
    std::counting_semaphore<> items_{0}; //counting semaphores go from 0 - inf
    std::counting_semaphore<> spaces_{Size};
    std::binary_semaphore mutex_{1}; //binary semaphore goes from 0 - 1, basically a mutex
};

}
