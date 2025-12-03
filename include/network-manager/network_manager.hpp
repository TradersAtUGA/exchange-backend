/**
 * @brief Network Manager to handle all inbound and outbound network connections
*/
#pragma once

#include <crow.h>
#include <moodycamel/concurrentqueue.h>
#include <thread>

#include "network-inbound/api_routing.hpp"
#include "network-inbound/data_conversion.hpp"
#include "network-inbound/data_validation.hpp"
#include "shared/order.hpp"


// Also include network outbound headers when they are complete

// quickfix library 
#include "exchange-gateway/gateway.hpp"

#include <quickfix/Acceptor.h>
#include <quickfix/FileLog.h>
#include <quickfix/FileStore.h>
#include <quickfix/SocketAcceptor.h>

namespace exchange 
{

class NetworkManager {
public:

    /**
     * @brief creates a new NetworkManager instance to control
     * both inbound and outbound network servers
     */
    NetworkManager();
    
    /**
     * @brief starts the NetworkManager inbound server instance
     * @param q the inbound network queue that will receive exchange::Order objects from users
     */
    void start_inbound_server(moodycamel::ConcurrentQueue<exchange::Order>& q);

    /**
     * @brief starts the NetworkManager outbound server instance
     */
    void start_outbound_server();

    /**
     * @brief stops the NetworkManager inbound server instance
     */
    void stop_inbound_server();

    /**
     * @brief stops the NetworkManager outbound server instance
     */
    void stop_outbound_server();

    /**
     * @brief starts the exchange gateway implementation from the quickfix 
     * engine library
     */
    void start_gateway();

private:
    // Server objects
    crow::SimpleApp inbound_server_;
    crow::SimpleApp outbound_server_;   
    std::thread inbound_server_thread_;
    std::thread outbound_server_thread_;

    ExchangeGateway application;
    FIX::SessionSettings settings;
    FIX::FileStoreFactory storeFactory;
    FIX::FileLogFactory logFactory;
    std::unique_ptr<FIX::SocketAcceptor> acceptor;
};
}