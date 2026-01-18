#include <iostream>

#include "gateway/gateway.hpp"
#include "matching-engine/matching_engine.hpp"

#define NL "\n"
using std::cout;

#include "quickfix/SocketAcceptor.h"
#include "quickfix/FileStore.h"
#include "quickfix/FileLog.h"
#include "quickfix/SessionSettings.h"

int main(int argc, char** argv) {

    // try to make a matching engine
    exchange::MatchingEngine me("XYZ");

    auto buyside = exchange::Order(
        Side::BUY, 
        OrderType::MARKET,
        TIF::DAY,
        "XYZ",
        10, // price
        5, // qty
        10,
        987654321,
        10,
        1
    );

    std::cout << "before processing" << NL;

    me.process(buyside);

    // FIX::SessionSettings settings("gateway.cfg");
    // Gateway app;
    // FIX::FileStoreFactory storeFactory(settings);
    // FIX::FileLogFactory logFactory(settings);
    // FIX::SocketAcceptor acceptor(app, storeFactory, settings, logFactory);

    // acceptor.start();
    // std::cout << "Exchange acceptor running..." << std::endl;
    // std::cin.get();  // wait for Enter
    // acceptor.stop();
}
