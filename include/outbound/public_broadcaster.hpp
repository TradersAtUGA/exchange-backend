#pragma once 

#include "outbound/public_broadcaster.hpp"

#include <cstdint> 
#include <array>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <atomic>

#include "shared/quote.hpp"
#include "shared/trade.hpp"
#include "shared/cancel.hpp"
#include "config.hpp"

namespace exchange {
    
class PublicBroadcaster {
public:

    PublicBroadcaster() { 
        setup_socket_(trade_socket_, 
            trade_dest_addr_, trade_addr_, trade_port_);
        setup_socket_(quote_socket_, 
            quote_dest_addr_, quote_addr_, quote_port_);
    }

    /** @brief starts consuming and publishing data */
    void start(); 

    /** @brief stops operation of the broadcaster */
    void stop(); 

private:

    /**
     * @brief setup a socket for multicast
     * 
     * @param socket socket to setup
     * @param dest_addr the sockaddr_in struct to setup
     * @param addr the IP address destination 
     * @param port the port number 
     */
    void setup_socket_(
        int32_t& socket, 
        sockaddr_in& dest_addr, 
        const char * addr, 
        int32_t port);

    /**
     * @brief publishes JSON as a string to receiver
     * 
     * @param socket the socket to send from 
     * @param dest_addr sockaddr_in struct associated with the socket
     * @param msg the message to send
     */
    void publish_(
        const int32_t& socket, 
        const sockaddr_in& dest_addr_,
        const std::string& msg
    );

    // sockets
    int32_t trade_socket_;
    int32_t quote_socket_; 
    sockaddr_in trade_dest_addr_; 
    sockaddr_in quote_dest_addr_; 

    // settings
    const char * trade_addr_ = config::PUBLIC_TRADE_IP_ADDR.c_str();
    const char * quote_addr_ = config::PUBLIC_QUOTE_IP_ADDR.c_str();
    const int32_t trade_port_ = config::PUBLIC_TRADE_PORT; 
    const int32_t quote_port_ = config::PUBLIC_QUOTE_PORT;

    unsigned char ttl_; 

    std::array<char, 2048> trade_buffer_; 
    std::array<char, 2048> quote_buffer_;

    std::atomic<uint8_t> running_; 
};



}