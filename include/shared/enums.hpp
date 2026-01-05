#pragma once
#include <cstdint>

/*
    Note that the integer values in the enums are mapped
    1-1 with FIX based values to see more values check:

    build/vcpkg_installed/x64-linux/include/quickfix/FixValues.h
*/

enum struct Side : uint8_t {
    BUY = 1,
    SELL = 2
};

enum struct OrderType : uint8_t {
    MARKET = 1, 
    LIMIT = 2,
};

enum struct TIF : uint8_t { 
    DAY = 0,
    IOC = 3, 
    FOK = 4
};

// Not being used in implementation right now
enum struct Trigger : uint8_t { 
    NONE = 0,
    STOP = 1
};