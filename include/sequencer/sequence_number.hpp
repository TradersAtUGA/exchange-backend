#pragma
#include <atomic>
#include <cstdint>
#include <memory>

namespace exchange {

class SequenceNumber {
public:
    explicit SequenceNumber(uint64_t start = 0);
    uint64_t next();
    uint64_t current() const;
    void reset(uint64_t start = 0);

private:
    std::atomic<uint64_t> counter_;
};

// factory for class
std::unique_ptr<SequenceNumber> create_sequence_number();

}