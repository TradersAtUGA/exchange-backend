#include <iostream>

#include "gateway/gateway.hpp"
#include "matching-engine/matching_engine.hpp"
#include "outbound/broadcaster.hpp"

#define NL "\n"
using std::cout;

#include "quickfix/SocketAcceptor.h"
#include "quickfix/FileStore.h"
#include "quickfix/FileLog.h"
#include "quickfix/SessionSettings.h"

using namespace exchange; 

int main(int argc, char** argv) {

    MatchingEngine me("XYZ"); 

    Order sellside_ = Order::create_new_order(
        Side::BUY, 
        OrderType::LIMIT, 
        TIF::DAY, 
        const_cast<char *>("XYZ"), 
        10, 
        10, 
        123456789, 
        1
    );

    std::cout << sellside_ << "<- the sellside_ order price" << std::endl;
    Message<Order> sellside(sellside_); 

    std::cout << sellside << std::endl;

    me.process(sellside); 

    std::cout << me.orderbook() << std::endl;

   
    return 0; 
    

}
