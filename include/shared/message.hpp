#pragma once 

#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/SessionID.h>
#include <quickfix/fix44/NewOrderSingle.h>
#include <cstdint>
#include <ostream>
#include "shared/utilities.hpp"

/** 
 * @brief wrapper for message types 
 * (orders, cancels, trades, etc.)
 * to hold metadata in addition to the message type. 
 */
template <typename T> 
struct Message {

    Message() {
        payload = T();
        FIX::SessionID dummy; 
        session_id = dummy; 
        recv_time = 0;
    }

    Message(T& payload, const FIX::SessionID& session_id)
        : payload(payload),
        session_id(session_id),
        recv_time(exchange::get_time_ms()) {}


    T payload; 
    FIX::SessionID session_id; // i.e. which broker this came from
    uint64_t recv_time; // when msg came into exchange

    // i dont think we can just print this out because the payload being printable is not known
    friend std::ostream& operator<<(std::ostream& os, const Message& m) {
        os << "temp" << "\n";
        return os;
    }

};  

