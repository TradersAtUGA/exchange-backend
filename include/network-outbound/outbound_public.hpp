/**
 * @brief outbound server endpoints via UDP for general 
 * public information
 */
#pragma once 
#include <crow.h>
#include <moodycamel/concurrentqueue.h>
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <atomic>

#include "shared/order.hpp"

namespace exchange {



class NetworkOutbound {
public:

    /**
     * @brief constructor for the network outbound 
     */
    NetworkOutbound();

    /**
     * @brief sets up and starts the public outbound server
     */
    void start_outbound_public_server();

     /**
      * @brief stops and closes the outbound server
      */
    void stop_outbound_public_server();

private:

    // outbound TCP server side socket
    socket outbound_server_socket_;

    // configuration struct for the server
    sockaddr_in server_addr_;


};
    
    
} // network_outbound


