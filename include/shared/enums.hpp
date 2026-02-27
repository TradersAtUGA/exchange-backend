#pragma once
#include <cstdint>
#include <cstring>

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
    // NOTE: this is not an actual order type associated with FIX
    // but a result from combining cancel and orders into a
    // single struct 
    CANCEL = 0 
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
enum struct Status : uint8_t {
    CANCELED = 4, 
    FILLED = 2, 
    PARTIALLY_FILLED = 1, 
    REJECTED = 8,
    NEW = 0
};

// Payload Type
// We need this since the Order Struct is shared for 
// Cancel and actual Orders and the cancel constructor 
// does not fill out order related methods
enum struct PayloadType : uint8_t { 
    ORDER = 0,
    CANCEL = 1
};

// IPC RingBuffer Selection
enum class IPCKey : uint8_t {
    INBOUND_TO_MATCHING_ENGINE     = 1,
    MATCHING_ENGINE_TO_BROADCASTER = 2,
    MATHCING_ENGINE_TO_INDIVIDUAL  = 3
};