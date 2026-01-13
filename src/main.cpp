#include "exchange/exchange_controller.hpp"
#include <atomic>
#include <iostream>
#include <signal.h>
#include "shared/logger.hpp"

std::atomic<bool> running(true);

void handle_sigint(int signal) {
    exchange::Logger::info("Signal Interrupt Caught in main");
    running.store(false, std::memory_order_release);
}

int main() {
    std::signal(SIGINT, handle_sigint);
    
    // Load in constants
    exchange::Exchange exchange(running);
    // Creates sequencer, matching engines, threads
    exchange.init();
    // Start networks loops, sequencer loop, 
    exchange.run(); 
    return 0;
}
