#include "exchange/exchange.hpp"
#include <atomic>
#include <iostream>
#include <signal.h>
#include "shared/debug.hpp"

std::atomic<bool> running(true);

void handle_sigint(int signal) {
    DEBUG_PRINT("Handle Sig called");
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