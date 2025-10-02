#pragma once
#include <cstdint>
#include <cstddef>

namespace exchange {

// Note for size make sure to keep it 2^n for fast wrapping
template<typename T, size_t Size>
class RingBuffer {
public:
    bool enqueue(T&& item); //rvals
    bool dequeue(T& item);
    bool empty() const;
    bool full() const;
private:
    T buffer_[Size];
    size_t head_, tail_, count_;
};
    
}