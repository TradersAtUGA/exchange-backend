#pragma once
#include <cstdint>
#include <cstddef>
#include <utility>

namespace exchange {

template<typename T, size_t Size>
class RingBuffer {
public:
    RingBuffer() {
        head_ = 0;
        tail_ = 0;
        count_ = 0;
    }

    bool enqueue(T&& item) {
        if (count_ == Size) {
            return false; // queue is full
        }
        buffer_[tail_] = std::move(item);
        tail_ = (tail_ + 1) % Size;
        ++count_;
        return true;
    }

    bool dequeue(T& item) {
        if (count_ == 0) {
            return false;
        }
        item = std::move(buffer_[head_]);
        head_ = (head_ + 1) % Size;
        --count_;
        return true;
    }

    bool empty() const {
        return (count_ == 0);
    }

    bool full() const {
        return (count_ == Size);
    }

private:
    T buffer_[Size];
    size_t head_, tail_, count_;
};

}
