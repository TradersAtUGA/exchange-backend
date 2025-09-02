#include <iostream>
#include "../include/network-input/api_routing.hpp"
#include "../include/order.hpp"
#include "crow.h"

int main() {
    // This is the main entrie of the engine
    
    // Place holder
    std::cout << "Docker is docking" << std::endl;
    std::cout << "Exchange is exchanging" << std::endl;

    crow::SimpleApp app;

    network_inbound::start_input_server(app);
}