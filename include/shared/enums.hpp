#pragma once
#include <cstdint>

/*
    Note that the integer values in the enums are mapped
    1-1 with FIX based values to see more values check:

    build/vcpkg_installed/x64-linux/include/quickfix/FixValues.h
*/

// Order's side 
enum struct Side : uint8_t {
    BUY = 1,
    SELL = 2
};

// Order's Type 
enum struct OrderType : uint8_t {
    MARKET = 1, 
    LIMIT = 2,
};

// Order's Time In Force
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

// Order status 
// not being used in implementation right now 
enum struct Status : uint8_t {
    DEAD = 0, 
    NO_FILL_YET = 1, 
    PARTIAL_FILL = 2, 
    CANNOT_FILL = 3,
    FILLED = 4,
    REJECTED = 5, 
};