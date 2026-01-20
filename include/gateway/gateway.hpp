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

    // Handles new outbound trade reports 

    // not implemented
    // void onMessage(const FIX44::ExecutionReport&, const FIX::SessionID&) override;


    void send_trade(const Trade& t, const FIX::SessionID& session_id); 

    void send_cancel_confirmation(const Cancel& c, const FIX::SessionID& session_id);

};
