/**
 * @brief Network Manager to handle all inbound and outbound network connections
*/
#include <crow.h>


#include "network-inbound/api_routing.hpp"
#include "network-inbound/data_conversion.hpp"
#include "network-inbound/data_validation.hpp"

// Also include network outbound headers when they are complete

namespace exchange 
{

class NetworkManager {
public:

    /**
     * @brief creates a new NetworkManager instance to control
     * both inbound and outbound network servers
     */
    NetworkManager();
    
    /**
     * @brief starts the NetworkManager inbound server instance
     */
    void start_inbound_server();

    /**
     * @brief starts the NetworkManager outbound server instance
     */
    void start_outbound_server();

    /**
     * @brief stops the NetworkManager inbound server instance
     */
    void stop_inbound_server();

    /**
     * @brief stops the NetworkManager outbound server instance
     */
    void stop_outbound_server();

private:
    // Server objects
    crow::SimpleApp inbound_server;
    crow::SimpleApp outbound_server;
};
}