/**
 * @brief gateway for placing FIX protocol based 
 * orders (replacing network-inbound/network-manager)
 */
#pragma once 
#include <quickfix/Application.h> 
#include <iostream>


class ExchangeGateway : public FIX::Application
{
public:
virtual ~ExchangeGateway() {};
virtual void onCreate( const FIX::SessionID& ) = 0;
virtual void onLogon( const FIX::SessionID& ) = 0;
virtual void onLogout( const FIX::SessionID& ) = 0;
virtual void toAdmin( FIX::Message&, const FIX::SessionID& ) = 0;

// override the throw() statements from quickfix library since they are no longer supported
virtual void toApp( FIX::Message&, const FIX::SessionID& ) noexcept override = 0;
virtual void fromAdmin( const FIX::Message&, const FIX::SessionID&) noexcept override = 0;
virtual void fromApp( const FIX::Message&, const FIX::SessionID&) noexcept override = 0;
};