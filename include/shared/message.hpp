#pragma once 

#include <quickfix/SessionID.h>
#include <cstdint>

/** 
 * @brief wrapper for message types 
 * (orders, cancels, trades, etc.)
 * to hold metadata in addition to the message type. 
 */
template <typename T> 
struct Message { 
    T payload; 
    const FIX44::SessionID session_id; // i.e. which broker this came from
    const uint64_t recv_time; // when msg came into exchange
};  

