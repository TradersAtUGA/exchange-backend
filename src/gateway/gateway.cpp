#include "gateway/gateway.hpp"
#include "quickfix/Field.h"
#include "quickfix/fix44/ExecutionReport.h"

void Gateway::onCreate(const FIX::SessionID& sessionID) {
    std::cout << "Session created: " << sessionID.toString() << std::endl;
}

void Gateway::onLogon(const FIX::SessionID& sessionID) {
    std::cout << "LOGON: " << sessionID.toString() << std::endl;
}

void Gateway::onLogout(const FIX::SessionID& sessionID) {
    std::cout << "LOGOUT: " << sessionID.toString() << std::endl;
}

void Gateway::toAdmin(FIX::Message& message, const FIX::SessionID&) {
    std::cout << "ADMIN S >> " << message.toString() << std::endl;
}

void Gateway::fromAdmin(const FIX::Message& message, const FIX::SessionID&) noexcept {
    std::cout << "ADMIN R << " << message.toString() << std::endl;
}

void Gateway::toApp(FIX::Message& message, const FIX::SessionID&) noexcept {
    std::cout << "APP   S >> " << message.toString() << std::endl;
}

void Gateway::fromApp(const FIX::Message& message, const FIX::SessionID& sessionID) noexcept {
    std::cout << "APP   R << " << message.toString() << std::endl;
    try {
        crack(message, sessionID);  // MessageCracker dispatch
    } catch (FIX::FieldNotFound& e) {
        std::cerr << "Field not found: " << e.what() << std::endl;
    } catch (FIX::IncorrectDataFormat& e) {
        std::cerr << "Data format error: " << e.what() << std::endl;
    } catch (FIX::IncorrectTagValue& e) {
        std::cerr << "Tag value error: " << e.what() << std::endl;
    } catch (FIX::UnsupportedMessageType& e) {
        std::cerr << "Unsupported message type: " << e.what() << std::endl;
    }
}

// ---- NewOrderSingle handler ----
void Gateway::onMessage(const FIX44::NewOrderSingle& msg, const FIX::SessionID&) {
    Order o;
    FIX::ClOrdID clOrdID;
    FIX::Symbol symbol;
    FIX::Side side;
    FIX::OrderQty qty;
    FIX::OrdType ordType;
    FIX::Price price;
    FIX::TimeInForce tif;

    msg.getField(clOrdID);
    msg.getField(side);
    msg.getField(qty);
    msg.getField(ordType);
    msg.getField(symbol);
    msg.getField(tif);

    o.recv_time = get_time_ms();
    
    // TODO assign order id 
    o.oid =  0; // SEQUENCER MUST ASSIGN THE VALUE HERE TO THE ORDER ID 
    o.cid = std::stoull(clOrdID.getValue());                  // assign based on sessionID
    
    if (side.getValue() == FIX::Side_BUY) {
        o.side = Side::BUY;
    } else if (side.getValue() == FIX::Side_SELL) {
        o.side = Side::SELL;
    }

    o.qty = double_to_uint64_t(qty.getValue());

    if (ordType.getValue() == FIX::OrdType_LIMIT) {
        o.order_type = OrderType::LIMIT;
        msg.getField(price);
        o.price = price.getValue();
    } else if (ordType.getValue() == FIX::OrdType_MARKET) {
        o.order_type = OrderType::MARKET;
    }

    if (tif.getValue() == FIX::TimeInForce_DAY) {
        o.tif = TIF::DAY;
    } else if (tif.getValue() == FIX::TimeInForce_FILL_OR_KILL) { 
        o.tif = TIF::FOK;
    } else if (tif.getValue() == FIX::TimeInForce_IMMEDIATE_OR_CANCEL) {
        o.tif = TIF::IOC;
    }

    o.ticker = symbol.getValue();

    o.status = 1;

    if (msg.isSetField(FIX::FIELD::TimeInForce))
        msg.getField(tif), o.tif = TIF::DAY;

    std::cout << "Received NewOrderSingle: order_id=" << o.oid
              << " symbol=" << symbol.getValue()
              << " side=" << static_cast<int>(o.side)
              << " qty=" << o.qty
              << " price=" << o.price
              << " tif=" << static_cast<int>(o.tif)
              << std::endl;

    // TODO: send to order ring buffer that feeds to matching engines
}

// ---- OrderCancelRequest handler ----
void Gateway::onMessage(const FIX44::OrderCancelRequest& msg, const FIX::SessionID&) {
    // CancelEvent c;
    FIX::OrigClOrdID origClOrdID;
    FIX::ClOrdID clOrdID;
    FIX::Side side;
    FIX::Symbol symbol;

    msg.getField(origClOrdID);
    msg.getField(clOrdID);
    msg.getField(side);
    msg.getField(symbol);

    // c.target_order_id = std::stoull(origClOrdID.getValue());
    // c.participant_id = 0;  // map from sessionID

    // std::cout << "Received CancelRequest: target_order_id=" << c.target_order_id
    //           << " symbol=" << symbol.getValue()
    //           << std::endl;

    // TODO: submit cancel to sequencer / matching engine
}