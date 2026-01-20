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
    const FIX44::SessionID session_id;
    const uint64_t recv_time;     
};  

