/**
 * @brief gateway for placing FIX protocol based 
 * orders (replacing network-inbound/network-manager)
 */
#pragma once 
#include <quickfix/Application.h> 
#include <quickfix/MessageCracker.h>
#include <iostream>


class ExchangeGateway 
    : public FIX::Application, public FIX::MessageCracker
{
public:

    ~ExchangeGateway() override {}

    void onCreate(const FIX::SessionID& session) override {
        std::cout << "onCreate\n";
    }
    void onLogon(const FIX::SessionID& session) override {
        std::cout << "onLogon\n";
    }
    void onLogout(const FIX::SessionID& session) override {
        std::cout << "onLogout\n";
    }
    void toAdmin(FIX::Message& msg, const FIX::SessionID& session) override {
        std::cout << "toAdmin\n";
    }
    void toApp(FIX::Message& msg, const FIX::SessionID& session) noexcept override {
        try {
            std::cout << "toAPP\n";
            crack(msg, session);
        } catch (const std::exception& e) {
            std::cerr << "toAPP error: " << e.what() << "\n";
        }
    }
    void fromAdmin(const FIX::Message& msg, const FIX::SessionID& session) noexcept override {
        try {
            std::cout << "Received admin message\n";
            crack(msg, session);
        } catch (const std::exception& e) {
            std::cerr << "Admin message error: " << e.what() << "\n";
        }
    }
    void fromApp(const FIX::Message& msg, const FIX::SessionID& session) noexcept override {
        try {
            std::cout << "Received application message\n";
            crack(msg, session);
        } catch (const std::exception& e) {
            std::cerr << "App message error: " << e.what() << "\n";
        }
    }
    
    void onMessage(const FIX42::NewOrderSingle& msg, const FIX::SessionID& session) override {
        std::cout << "NewOrderSingle received\n";
    }
};
