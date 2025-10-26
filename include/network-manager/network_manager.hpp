/**
 * @brief Network Manager to handle all inbound and outbound network connections
*/
#include <crow.h>
#include <moodycamel/concurrentqueue.h>
#include <thread>

#include "network-inbound/api_routing.hpp"
#include "shared/order.hpp"
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
     * @param q the inbound network queue that will receive exchange::Order objects from users
     */
    void start_inbound_server(moodycamel::ConcurrentQueue<exchange::Order>& q);

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
    crow::SimpleApp inbound_server_;
    crow::SimpleApp outbound_server_;   
    std::thread inbound_server_thread_;
    std::thread outbound_server_thread_;
};
}