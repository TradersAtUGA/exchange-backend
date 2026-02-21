#pragma once 

#include <cstdint>
#include <array>
 
#include "shared/quote.hpp"
#include "shared/trade.hpp"
#include "shared/order.hpp"
#include "shared/message.hpp"

using namespace exchange;

/** 
 * @brief Represents publishable data that can be fed into the broadcaster
 * for reporting 
 * */
struct PublishableData {

    /** 
     * @brief contains information about the filled or 
     * partially filled bid and ask orders 
     * 
     * @note the bid order will be index 0, and 
     * and the ask order will be at index 1
     */
    std::array<Message<Order>, 2> filled_orders;
    
    /** @brief contains information about the latest price information for the given ticker */
    Quote quote; 

    /** @brief contains information about trade information between both orders */
    Trade trade; 

};