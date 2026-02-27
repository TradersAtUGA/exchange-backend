#pragma once

#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cstdint>

namespace sim {

class PerfTimer {
public:
    void start() { start_ = std::chrono::high_resolution_clock::now(); }

    uint64_t stop_ns() {
        auto end = std::chrono::high_resolution_clock::now();
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_).count();
        samples_.push_back(static_cast<uint64_t>(ns));
        return static_cast<uint64_t>(ns);
    }

    double avg_ns() const {
        if (samples_.empty()) return 0;
        return std::accumulate(samples_.begin(), samples_.end(), 0.0) /
               static_cast<double>(samples_.size());
    }

    uint64_t p99_ns() {
        if (samples_.empty()) return 0;
        std::sort(samples_.begin(), samples_.end());
        return samples_[static_cast<size_t>(static_cast<double>(samples_.size()) * 0.99)];
    }

    uint64_t p50_ns() {
        if (samples_.empty()) return 0;
        std::sort(samples_.begin(), samples_.end());
        return samples_[samples_.size() / 2];
    }

    size_t count() const { return samples_.size(); }
    void clear() { samples_.clear(); }

private:
    std::chrono::high_resolution_clock::time_point start_;
    std::vector<uint64_t> samples_;
};

} // namespace sim
