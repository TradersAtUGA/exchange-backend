#pragma once 

#include <cstdint> 
#include <array>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <atomic>
#include <thread>
#include <chrono>

#include "gateway/gateway.hpp"

#include "shared/quote.hpp"
#include "shared/trade.hpp"
#include "shared/cancel.hpp"
#include "config.hpp"

namespace exchange {

/**  
 * @brief handles transmission of quotes, trades, and cancel confirmations
 * to both public and private clients 
 */
class Broadcaster {
public:

    /** 
     * @brief creates a new Broadcaster object
     * 
     * @param gateway the main Gateway the exchange is relying on 
     * to handle FIX related issues
     * @return a new Broadcaster object 
     */
    Broadcaster(Gateway* gateway) : gate_ptr(gateway) { 
        setup_socket_(trade_socket_, 
            trade_dest_addr_, trade_addr_, trade_port_);
        setup_socket_(quote_socket_, 
            quote_dest_addr_, quote_addr_, quote_port_);
    }

    ~Broadcaster(); 

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
     * @brief begins publishing threads
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

    /** 
     * @brief (on a new thread) consumes and publishes trade
     * data from the inbound trade buffer, to both public and private
     * clients via multicast and the gateway, respectively
     */
    void publish_trades_(std::atomic<uint8_t>& running); 

    /**
     * @brief (on a new thread) consumes and publishes quote
     * data from the inbound quote buffer, to public clients via 
     * multicast
     */
    void publish_quotes_(std::atomic<uint8_t>& running); 

    /** 
     * @brief (on a new thread) consumes and publishes cancel 
     * confirmation from the inbound cancel buffer, to private
     * clients via the gateway 
     */
    void publish_cancels_(std::atomic<uint8_t>& running); 

    /** @brief joins all threads that were created in start() */
    void join_threads_(); 

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
    unsigned char ttl_; // time to live for multi cast

    // UNUSED
    std::array<char, 2048> trade_buffer_; 
    std::array<char, 2048> quote_buffer_;

    // flag for all running threads
    std::atomic<uint8_t> running_; 

    // pointer to exchange gateway (needed to call FIX hooks)
    Gateway* gate_ptr; 

    // worker threads
    std::thread trade_thread_;
    std::thread quote_thread_;
    std::thread cancel_thread_;
};

}