#include "outbound/broadcaster.hpp"

#include <iostream>

using namespace exchange; 

void Broadcaster::start() { 
    running_ = 1; 

    // start running consumer producers on new threads
    trade_thread_ = std::thread(&Broadcaster::publish_trades_, this, std::ref(running_));
    quote_thread_ = std::thread(&Broadcaster::publish_quotes_, this, std::ref(running_));
    cancel_thread_ = std::thread(&Broadcaster::publish_cancels_, this, std::ref(running_));
}

void Broadcaster::stop() { 
    running_ = 0;       
    join_threads_();  
    close(trade_socket_); 
    close(quote_socket_); 
}

Broadcaster::~Broadcaster() {
    running_ = 0; 
    join_threads_(); 
}

void Broadcaster::join_threads_() { 
    if (trade_thread_.joinable()) trade_thread_.join(); 
    if (quote_thread_.joinable()) quote_thread_.join(); 
    if (cancel_thread_.joinable()) cancel_thread_.join();  
}

void Broadcaster::publish_(
    const int32_t& socket, 
    const sockaddr_in& dest_addr, 
    const std::string& msg) {
    if (sendto(socket, msg.c_str(), msg.size(), 0,
        (sockaddr*)&dest_addr, sizeof(dest_addr)) < 0) {
            perror("sendto");
    } else {
        std::cout << "Sent: " << msg << std::endl;
    }
}

void Broadcaster::setup_socket_(
    int32_t& sock, 
    sockaddr_in& dest_addr, 
    const char * addr, 
    int32_t port
) {

    sock = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sock < 0) {
        perror("socket"); 
        std::terminate(); 
    }

    ttl_ = 1; 
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl_, sizeof(ttl_)) < 0) {
        perror("setsockopt TTL");
        close(sock);
        std::terminate();
    } 

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, addr, &dest_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        std::terminate(); 
    }

    unsigned char loop = 1;
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0) {
        perror("setsockopt LOOP");
        close(sock);
        std::terminate();
    }   
}

/*
These three methods should try dequeue from some inbound 
ring buffer and publish the data to their respective
 destinations 
*/

void Broadcaster::publish_trades_(std::atomic<uint8_t>& running) { 
    while(running) { 
        std::cout << "trade" << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


void Broadcaster::publish_quotes_(std::atomic<uint8_t>& running) { 
    while(running) { 
        std::cout << "quote" << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Broadcaster::publish_cancels_(std::atomic<uint8_t>& running) { 
    while(running) { 
        std::cout << "cancel" << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}