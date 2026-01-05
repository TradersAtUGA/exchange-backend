#pragma once
#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/SessionID.h>
#include <quickfix/fix44/NewOrderSingle.h>
#include <quickfix/fix44/OrderCancelRequest.h>
#include <iostream>
#include <string>
#include <cstdint>

#include "shared/order.hpp"
#include "shared/utilities.hpp"
#include "shared/cancel.hpp"

class Gateway: public FIX::Application, public FIX::MessageCracker
{
public:
    void onCreate(const FIX::SessionID& sessionID) override;
    void onLogon(const FIX::SessionID& sessionID) override;
    void onLogout(const FIX::SessionID& sessionID) override;
    void toAdmin(FIX::Message&, const FIX::SessionID&) override;
    void fromAdmin(const FIX::Message&, const FIX::SessionID&) noexcept override;
    void toApp(FIX::Message&, const FIX::SessionID&) noexcept override;
    void fromApp(const FIX::Message&, const FIX::SessionID&) noexcept override;

    // MessageCracker handlers
    void onMessage(const FIX44::NewOrderSingle&, const FIX::SessionID&) override;
    void onMessage(const FIX44::OrderCancelRequest&, const FIX::SessionID&) override;
};
