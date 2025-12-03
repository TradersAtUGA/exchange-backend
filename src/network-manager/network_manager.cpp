#include <crow.h>
#include <moodycamel/concurrentqueue.h>
#include <thread>

#include "network-manager/network_manager.hpp"
#include "network-inbound/api_routing.hpp"
#include "network-inbound/data_conversion.hpp"
#include "network-inbound/data_validation.hpp"
#include "shared/order.hpp"

#include <iostream>



namespace exchange 
{

NetworkManager::NetworkManager()
    : settings("settings.cfg"),
      storeFactory(settings),
      logFactory(settings)
{}

void NetworkManager::start_inbound_server(moodycamel::ConcurrentQueue<exchange::Order>& q) {
    inbound_server_thread_ = std::thread([this, &q]() { // capture instance of this object and the queue by ref
        network_inbound::start_input_server(inbound_server_, q);
    }); 
}

void NetworkManager::start_outbound_server() {
    return; // TODO: Implement
}

void NetworkManager::stop_inbound_server() {
    inbound_server_.stop(); // stop crow server first
    inbound_server_thread_.join(); // led thread finish cleanly then join
} 

void NetworkManager::stop_outbound_server() {
    return; // TODO: Implement
}

void NetworkManager::start_gateway() {
    acceptor = std::make_unique<FIX::SocketAcceptor>(application, storeFactory, settings, logFactory);
    acceptor->start();
}

}