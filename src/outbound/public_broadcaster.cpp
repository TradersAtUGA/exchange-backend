#include "outbound/public_broadcaster.hpp"

#include <iostream>

using namespace exchange; 

// one thread will push into a outbound queue
// another thread will push trade info into a outbound queue 
// a sender thread will try dequeue and send infomration out 

void PublicBroadcaster::start() { 
    running_ = 1; 
    Quote q = {"A", 1, 2, 3, 4};
    while (running_) { 

        // try dequeue from ring buffer

        // assert type 

        // depending on type send to quote or trade socket 

        // TODO(vikas): implement a interface for all structs
        // to have a to_json and type method 

      publish_(trade_socket_, trade_dest_addr_, q.to_json());
    }
}

void PublicBroadcaster::stop() { 
    running_ = 0; 
    close(trade_socket_); 
    close(quote_socket_); 
}

void PublicBroadcaster::publish_(
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

void PublicBroadcaster::setup_socket_(
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