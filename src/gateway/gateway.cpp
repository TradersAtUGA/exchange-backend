#include "gateway/gateway.hpp"
#include "quickfix/Field.h"
#include "quickfix/fix44/ExecutionReport.h"
#include "shared/cancel.hpp"
#include "gateway/validation.hpp"
#include "shared/message.hpp"

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

void Gateway::fromAdmin(const FIX::Message& message, const FIX::SessionID& sessionID) noexcept {
    std::cout << "ADMIN R << " << message.toString() << std::endl;
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
    } catch (FIX::FieldConvertError& e) {
        std::cerr << "Could not convert some field: " << e.what() << std::endl;
    } catch (FIX::SocketRecvFailed& e) {
        std::cerr << "Connection reset by peer " << e.what() << std::endl;
    }
}

void Gateway::toApp(FIX::Message& message, const FIX::SessionID& sessionID) noexcept {
    std::cout << "APP   S >> " << message.toString() << std::endl;
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
    } catch (FIX::FieldConvertError& e) {
        std::cerr << "Could not convert some field: " << e.what() << std::endl;
    } catch (FIX::SocketRecvFailed& e) {
        std::cerr << "Connection reset by peer " << e.what() << std::endl;
    }
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
    } catch (FIX::FieldConvertError& e) {
        std::cerr << "Could not convert some field: " << e.what() << std::endl;
    } catch (FIX::SocketRecvFailed& e) {
        std::cerr << "Connection reset by peer " << e.what() << std::endl;
    }
}

// ---- NewOrderSingle handler ----
void Gateway::onMessage(const FIX44::NewOrderSingle& msg, const FIX::SessionID& sessionID) {
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
    msg.getField(price);

    if (exchange::validate_fix_values(
        side.getValue(),
        ordType.getValue(),
        tif.getValue(), 
        price.getValue(),
        qty.getValue()) == 0) return; 
    // TODO(vikas): this should call another method that rejects the order
    // since the order was malformed back to the broker 

    exchange::Order o = exchange::generate_order(
        side.getValue(),
        ordType.getValue(),
        tif.getValue(),
        price.getValue(),
        qty.getValue(),
        clOrdID.getValue()
    );

    std::cout << "Received NewOrderSingle: order_id=" << o.oid
              << " symbol=" << symbol.getValue()
              << " side=" << static_cast<int>(o.side)
              << " qty=" << o.qty
              << " price=" << o.price
              << " tif=" << static_cast<int>(o.tif)
              << std::endl;

    Message<exchange::Order> msg = {o, sessionID, exchange::get_time_ms()};

    // TODO: send to order ring buffer that feeds to matching engines
}

// ---- OrderCancelRequest handler ----
void Gateway::onMessage(const FIX44::OrderCancelRequest& msg, const FIX::SessionID&) {
    FIX::OrigClOrdID origClOrdID;
    FIX::ClOrdID clOrdID;
    FIX::Side side;
    FIX::Symbol symbol;

    msg.getField(origClOrdID);
    msg.getField(clOrdID);
    msg.getField(side);
    msg.getField(symbol);

    // Cancel c;

    // c.cancel_id = 0; // SEQUENCER should assign value here
    // // TODO(vikas): order struct should contain a order id that was used internally from the brokerage
    // c.order_id = 0; 
    // c.client_id = 1; // this will just be the broker id same as the one above for orders

    // c.target_order_id = std::stoull(origClOrdID.getValue());
    // c.participant_id = 0;  // map from sessionID

    // std::cout << "Received CancelRequest: target_order_id=" << c.target_order_id
    //           << " symbol=" << symbol.getValue()
    //           << std::endl;

    // TODO: submit cancel to sequencer / matching engine
}

void Gateway::send_trade(const Trade& trade, const FIX::SessionID& sessionID) {
    // FIX44::ExecutionReport execReport(
    //     FIX::OrderID(trade.bcid),
    //     FIX::ExecID(trade.tid),
    //     FIX::ExecType(FIX::ExecType_FILL),
    //     FIX::OrdStatus(FIX::OrdStatus_FILLED),
    //     FIX::Side(trade.side),
    //     FIX::LeavesQty(0),
    //     FIX::CumQty(trade.qty),
    //     FIX::AvgPx(trade.price)
    // )

    // // Required / common fields
    // execReport.set(FIX::ClOrdID(trade.cl_ord_id));
    // execReport.set(FIX::Symbol(trade.symbol));
    // execReport.set(FIX::LastQty(trade.qty));
    // execReport.set(FIX::LastPx(trade.price));
    // execReport.set(FIX::OrderQty(trade.qty));
    // execReport.set(FIX::TransactTime());
    // execReport.set(FIX::Account(trade.account));

    // FIX::Session::sendToTarget(execReport, sessionID);

    // STUB
    return; 
}