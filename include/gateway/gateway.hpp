#pragma once
#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/SessionID.h>
#include <quickfix/fix44/NewOrderSingle.h>
#include <quickfix/fix44/OrderCancelRequest.h>
#include <quickfix/fix44/ExecutionReport.h>
#include <iostream>
#include <string>
#include <cstdint>

#include "shared/order.hpp"
#include "shared/utilities.hpp"
#include "shared/cancel.hpp"
#include "shared/trade.hpp"
#include "shared/message.hpp"

class Gateway: public FIX::Application, public FIX::MessageCracker
{
public:
    // QUICKFIX required overrides
    void onCreate(const FIX::SessionID& sessionID) override;
    void onLogon(const FIX::SessionID& sessionID) override;
    void onLogout(const FIX::SessionID& sessionID) override;
    void toAdmin(FIX::Message&, const FIX::SessionID&) override;
    void fromAdmin(const FIX::Message&, const FIX::SessionID&) noexcept override;
    void toApp(FIX::Message&, const FIX::SessionID&) noexcept override;
    void fromApp(const FIX::Message&, const FIX::SessionID&) noexcept override;

    // Custom handlers (Message Crackers)

    // Handles new inbound orders -- validates and pushes to matching engine queue
    void onMessage(const FIX44::NewOrderSingle&, const FIX::SessionID&) override;

    // Handles new inbound cancel requests -- validates and pushes matching engine queue
    void onMessage(const FIX44::OrderCancelRequest&, const FIX::SessionID&) override;

    // Handles sending trades back to the broker  
    void send_trade(const Trade& t, const FIX::SessionID& session_id); 

    // Handles orders in the matching engine that cannot be filled 
    // can also be called if a order reaches the bottom of the orderbook side that it was walking down 
    void send_order_cancel(const Message<exchange::Order> msg, const FIX::SessionID& session_id); 

    // Handles sending cancel confirmations back to the broker 
    void send_cancel(const Cancel& c, const FIX::SessionID& session_id);

    void send_rejection(const exchange::Order& o, const FIX::SessionID& session_id);
    
    

};
