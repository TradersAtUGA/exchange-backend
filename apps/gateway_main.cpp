#include <iostream>

#include "gateway/gateway.hpp"

#include "quickfix/SocketAcceptor.h"
#include "quickfix/FileStore.h"
#include "quickfix/FileLog.h"
#include "quickfix/SessionSettings.h"

// starts gateway to accept incoming orders from brokers
int main() {

    FIX::SessionSettings settings("gateway.cfg");
    Gateway app;
    FIX::FileStoreFactory storeFactory(settings);
    FIX::FileLogFactory logFactory(settings);
    FIX::SocketAcceptor acceptor(app, storeFactory, settings, logFactory);

    acceptor.start();
    std::cout << "Exchange acceptor running..." << "\n";
    std::cout << "press ENTER to stop" << "\n";
    std::cin.get();  // wait for Enter
    acceptor.stop();
    return 0;
}