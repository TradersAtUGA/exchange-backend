#include <iostream>
#include "../include/network-input/api_routing.hpp"
#include "crow.h"

int main() {
    crow::SimpleApp app;

    // TODO: put this on its own thread 
    // this will block 
    network_input::start_input_server(app); // start input server
}