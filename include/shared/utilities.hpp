#pragma once 

#include <cstdint>
#include <cstdlib>
#include <chrono>
#include <bit>

using std::uint64_t;

namespace exchange {

/** @brief **bit casts** a double value into a uint64_t */
inline double double_to_uint64_t(double value) { 
    return std::bit_cast<uint64_t>(value); 
}       

/** @brief **bit casts** a uint64_t value into a double */
inline uint64_t uint64_t_to_double(uint64_t value) {
    return std::bit_cast<double>(value);
}

/** @brief returns current unix time in ns */
inline uint64_t get_time_ns() {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count()
    );
}

/** @brief returns current unix time in ms */
inline uint64_t get_time_ms() {
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count()
    );
}   

/** @brief compiler agnostic unreachable method  */
[[noreturn]] inline void unreachable() noexcept {
#if defined(__GNUC__) || defined(__clang__)
    __builtin_unreachable();
#elif defined(_MSC_VER) // for MSVC
    __assume(false);
#else
    std::abort(); // fallback
#endif
}

}