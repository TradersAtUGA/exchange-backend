#pragma once 

#include <cstdint> 

#include "shared/quote.hpp"
#include "shared/trade.hpp"
#include "shared/cancel.hpp"

namespace exchange {

// stuff going back to FIX/broker
class PrivateBroadcaster {
public:

    void start(); 

    void stop(); 

    /**
     * @brief publishes client specific trade 
     * back to the broker (via gateway)
     * 
     * @param t the trade confirmation to send
     */
    void publish_trade(Trade t); 

    /**
     * @brief publishes client specific cancel 
     * confirmation back to the broker (via gateway)
     * 
     * @param c the cancel confirmation to send 
     */
    void publish_cancel_confirmation(Cancel c); 

};

}