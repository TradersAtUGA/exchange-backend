 #include "network-outbound/outbound_public.hpp"

#include <crow.h>
#include <moodycamel/concurrentqueue.h>
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "shared/order.hpp"
#include "config.hpp"

namespace exchange{


void NetworkOutbound::start_outbound_public_server() {
    
    outbound_server_socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(config::NETWORK_OUTBOUND_PUBLIC_PORT_NUMBER);
    server_addr_.sin_addr.s_addr = inet_addr("127.0.0.1");

    

}
    




} // network_outbound