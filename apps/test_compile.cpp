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

int main(int argc, char** argv) {

    Gateway app; 

    exchange::Broadcaster pub(&app);

    pub.start();

    std::this_thread::sleep_for(std::chrono::seconds(5));

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
