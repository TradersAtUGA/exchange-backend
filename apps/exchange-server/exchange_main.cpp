#include "exchange/exchange.hpp"
#include <atomic>
#include <iostream>
#include <signal.h>
#include "shared/debug.hpp"

std::atomic<bool> running(true);

void handle_sigint(int signal) {
    DEBUG_PRINT("Handle Sig called");
    running = false;
}

int main() {
    std::signal(SIGUSR1, handle_sigint);

    std::cout << "PID: " << getpid() << std::endl;
    
    exchange::Exchange exchange;
    // Creates sequencer, matching engines, threads
    exchange.init();
    // Start networks loops, sequencer loop, 
    exchange.run(running); 
    return 0;
}