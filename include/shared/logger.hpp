#pragma once
#include <cstdio>
#include <utility>

#ifdef ENABLE_LOGGING
constexpr bool enable_logging {true};
#else 
constexpr bool enable_logging {false};
#endif

namespace exchange {


class Logger {
public:
    template <typename... Args>
    static void info(const char* fmt, Args&&... args) {
        if constexpr (enable_logging) {
            std::printf("[INFO] ");
            std::printf(fmt, std::forward<Args>(args)...);
            std::printf("\n");
        }    
        return;
    }
    
    template <typename... Args>
    static void error(const char* fmt, Args&&... args) {
        if constexpr (enable_logging) {
            std::fprintf(stderr, "[ERROR] ");
            std::fprintf(stderr, fmt, std::forward<Args>(args)...);
            std::fprintf(stderr, "\n");
        }    
        return;
    }
};


} // namespace exchnge

