#pragma once 

#include <cstdint> 
#include <quickfix/SessionID.h>

struct Rejection { 
    const uint64_t cid; // client side order ID
    const uint64_t oid; // internal order ID 
    const FIX44::SessionID session_id; // route back to proper broker
};