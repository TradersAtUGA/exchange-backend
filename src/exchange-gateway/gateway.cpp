#include <quickfix/Application.h>
#include <iostream>

#include "exchange-gateway/gateway.hpp"

void ExchangeGateway::onCreate(const FIX::SessionID& session) {
    std::cout << "Session created: " << session.toString() << "\n";
}

void ExchangeGateway::onLogon(const FIX::SessionID& session) {
    std::cout << "Logon: " << session.toString() << "\n";
}

void ExchangeGateway::onLogout(const FIX::SessionID& session) {
    std::cout << "Logout: " << session.toString() << "\n";
}

void ExchangeGateway::toAdmin(FIX::Message& msg, const FIX::SessionID& session) {
    std::cout << "Sending admin message\n";
}

void ExchangeGateway::toApp(FIX::Message& msg, const FIX::SessionID& session) noexcept {
    std::cout << "Sending application message\n";
}

void ExchangeGateway::fromAdmin(
    const FIX::Message& msg, const FIX::SessionID& session) noexcept
{
    std::cout << "Received admin message\n";
}

void ExchangeGateway::fromApp(
    const FIX::Message& msg, const FIX::SessionID& session) noexcept
{
    std::cout << "Received application message\n";
}