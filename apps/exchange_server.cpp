#include <iostream>
#include "../include/network-input/api_routing.hpp"
#include "../include/order.hpp"
#include "crow.h"
#include "moodycamel/concurrentqueue.h"
#include <thread>


moodycamel::ConcurrentQueue<exchange::Order> q;

int main() {
    crow::SimpleApp app;

    // TODO: put this on its own thread 
    // this will block 

    network_input::start_input_server(app, q);
  

}