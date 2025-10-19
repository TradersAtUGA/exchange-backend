#include <include/exchange/exchange.hpp>
#include <atomic>
#include <csignal>

std::atomic<bool> running{true};

void handle_sigint(int) {
    running = false;
}

int main() {
    std::signal(SIGINT, handle_sigint);
    
    exchange::Exchange exchange;
    // Creates sequencer, matching engines, threads
    exchange.init();
    // Start networks loops, sequencer loop, 
    exchange.run(running); 
    return 0;
}