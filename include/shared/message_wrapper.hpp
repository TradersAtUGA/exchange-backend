#pragma once 

#include <quickfix/SessionID.h>
#include <cstdint>

/** 
 * @brief wrapper for message types 
 * (orders, cancels, trades, etc.)
 * to hold metadata in addition to the message type. 
 */
template <typename T> 
struct MessageWrapper { 
    T payload; 
    FIX44::SessionID session_id;
    uint64_t recv_time;     
};  

