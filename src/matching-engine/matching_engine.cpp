#include "../../pch.h"

#include "matching-engine/matching_engine.hpp"
#include <algorithm> // think we can remove this 
#include <tuple>

#include "shared/utilities.hpp"

#include <iostream> // DEBUG 

void exchange::MatchingEngine::process(Message<Order>& msg) { 
    mkt_to_lim_(msg.payload); // conv mkt -> lim 

    std::cout << "MATCHING FUNCTION CALL: " << 
    static_cast<int>(side_lut_converter_(msg.payload.side)) 
    << ", " << static_cast<int>(order_type_lut_converter_(msg.payload.ord_type)) 
    << ", " << static_cast<int>(tif_lut_converter_(msg.payload.tif)) 
    << "\n";

    (this->*MATCH_FUNC_LUT
        [side_lut_converter_(msg.payload.side)]
        [order_type_lut_converter_(msg.payload.ord_type)]
        [tif_lut_converter_(msg.payload.tif)]
    )(msg);
}

Message<Trade> exchange::MatchingEngine::generate_trade_(
    const exchange::Order& bid_order, 
    const exchange::Order& ask_order, 
    uint64_t filled_qty, 
    uint64_t trade_price
) {

    Trade t = Trade(
        bid_order.coid, // TODO: change later for actual val
        ask_order.coid,
        static_cast<uint64_t>(1), // this is the trade id we will need some static increment variable
        filled_qty,
        exchange::uint64_t_to_double(trade_price), // price was originally bitcasted 
        bid_order.ticker
    );

    // TODO: IMPLEMENT ACTUAL LOGIC 
    FIX::SessionID dummy;
    Message<Trade> msg = Message<Trade>(t, dummy);
    return msg; 
    // proper sessionID

}

void exchange::MatchingEngine::match_DAY_sell_(Message<exchange::Order>& msg) {

    auto it{ orderbook_.bids().begin() };
    // it->first = price
    // it->second = PriceLevel&

    while (it != orderbook_.bids().end() && msg.payload.qty > 0) { 
        if (it->first < msg.payload.price 
                && msg.payload.ord_type != OrderType::MARKET) {  
            // add what ever is left on the order
            // useful for init of orderbook if this is the first order coming in 
            on_partial_fill_aggressive_limit_(msg);
            return;
        }

        while(!it->second.empty() && msg.payload.qty > 0) {

            Message<exchange::Order>& match = it->second.front(); // first order at this price 
            PriceLevel& level = it->second;

            if (msg.payload.status == Status::CANCELED) { // if order dead (canceled)
                // should be renamed to canceled since cancel order already had their qty removed from the orderbook so it makes more sense we are just dequeueing
                level.consume_front(0);
                continue;
            }

            const uint64_t filled_qty{ std::min(msg.payload.qty, match.payload.qty) };
            const uint64_t fill_price{ match.payload.price };

        
            volume_ += filled_qty;
            match.payload.qty -= filled_qty;
            msg.payload.qty -= filled_qty; 

            std::cout << 
            level.total_shares() << " - " << filled_qty << " = " << 
            level.total_shares() - filled_qty << std::endl;

            if (match.payload.qty == 0) { 
                orderbook_.remove_order_ptr(match.payload.oid);
                level.consume_front(filled_qty);    
                match.payload.status = Status::FILLED; 
            } else level.reduce_shares(filled_qty);

            Quote q = generate_quote_(
                ticker_, fill_price, orderbook_.best_bid(), orderbook_.best_ask(), volume_
            );
            
            Message<Trade> t = generate_trade_(
                msg.payload, match.payload, filled_qty, fill_price
            );

        } 

        ++it; // move to next price
    } 
    if (msg.payload.ord_type != OrderType::MARKET) on_partial_fill_aggressive_limit_(msg);
}

void exchange::MatchingEngine::match_DAY_buy_(Message<exchange::Order>& msg) {

    auto it{ orderbook_.asks().begin() };
    // it->first = price
    // it->second = PriceLevel&

    while (it != orderbook_.asks().end() && msg.payload.qty > 0) { 
        if (it->first > msg.payload.price 
                && msg.payload.ord_type != OrderType::MARKET) {  
            // add what ever is left on the order
            // useful for init of orderbook if this is the first order coming in 
            on_partial_fill_aggressive_limit_(msg);
            return;
        }

        while(!it->second.empty() && msg.payload.qty > 0) {

            Message<exchange::Order>& match = it->second.front(); // first order at this price 
            PriceLevel& level = it->second;

            if (msg.payload.status == Status::CANCELED) { // if order dead (canceled)
                // should be renamed to canceled since cancel order already had their qty removed from the orderbook so it makes more sense we are just dequeueing
                level.consume_front(0);
                continue;
            } 

            const uint64_t filled_qty{ std::min(msg.payload.qty, match.payload.qty) };
            const uint64_t fill_price{ match.payload.price };

            volume_ += filled_qty;
            match.payload.qty -= filled_qty;
            msg.payload.qty -= filled_qty;

            if (match.payload.qty == 0) { 
                orderbook_.remove_order_ptr(match.payload.oid);
                level.consume_front(filled_qty);    
                match.payload.status = Status::FILLED; 
            } else level.reduce_shares(filled_qty);

            Quote q = generate_quote_(
                ticker_, fill_price, orderbook_.best_bid(), orderbook_.best_ask(), volume_
            );
            
            Message<Trade> t = generate_trade_(
                 match.payload, msg.payload, filled_qty, fill_price
            );

        } 

        ++it; // move to next price
    } 
    if (msg.payload.ord_type != OrderType::MARKET) on_partial_fill_aggressive_limit_(msg);
}

void exchange::MatchingEngine::on_partial_fill_aggressive_limit_(Message<Order>& o) {
    if (o.payload.qty > 0) orderbook_.add_order(o);
}


void exchange::MatchingEngine::match_IOC_sell_(Message<exchange::Order>& msg) {

    auto it{ orderbook_.bids().begin() };
    // it->first = price
    // it->second = PriceLevel&

    while (it != orderbook_.bids().end() && msg.payload.qty > 0) { 
        if (it->first < msg.payload.price) {  
            return;
        }

        while(!it->second.empty() && msg.payload.qty > 0) {

            Message<exchange::Order>& match = it->second.front(); // first order at this price 
            PriceLevel& level = it->second;

            if (msg.payload.status == Status::CANCELED) { // if order dead (canceled)
                // should be renamed to canceled since cancel order already had their qty removed from the orderbook so it makes more sense we are just dequeueing
                level.consume_front(0);
                continue;
            }

            const uint64_t filled_qty{ std::min(msg.payload.qty, match.payload.qty) };
            const uint64_t fill_price{ match.payload.price };

            volume_ += filled_qty;
            match.payload.qty -= filled_qty;
            msg.payload.qty -= filled_qty;

            if (match.payload.qty == 0) { 
                orderbook_.remove_order_ptr(match.payload.oid);
                level.consume_front(filled_qty);    
                match.payload.status = Status::FILLED; 
            }  else level.reduce_shares(filled_qty);

            Quote q = generate_quote_(
                ticker_, fill_price, orderbook_.best_bid(), orderbook_.best_ask(), volume_
            );
            
            Message<Trade> t = generate_trade_( // check if the call to all of these methods is correct since we need to ensure that we are not accidentally swapping orders when we should not be doing that 
                match.payload, msg.payload,  filled_qty, fill_price
            );

        } 
        ++it; // move to next price
    } 
}

void exchange::MatchingEngine::match_IOC_buy_(Message<exchange::Order>& msg) {

    auto it{ orderbook_.asks().begin() };
    // it->first = price
    // it->second = PriceLevel&

    while (it != orderbook_.asks().end() && msg.payload.qty > 0) { 
        if (it->first < msg.payload.price) {  
            return;
        }

        while(!it->second.empty() && msg.payload.qty > 0) {

            Message<exchange::Order>& match = it->second.front(); // first order at this price 
            PriceLevel& level = it->second;

            if (msg.payload.status == Status::CANCELED) { // if order dead (canceled)
                // should be renamed to canceled since cancel order already had their qty removed from the orderbook so it makes more sense we are just dequeueing
                level.consume_front(0);
                continue;
            }

            const uint64_t filled_qty{ std::min(msg.payload.qty, match.payload.qty) };
            const uint64_t fill_price{ match.payload.price };

            volume_ += filled_qty;
            match.payload.qty -= filled_qty;
            msg.payload.qty -= filled_qty;

            if (match.payload.qty == 0) { 
                orderbook_.remove_order_ptr(match.payload.oid);
                level.consume_front(filled_qty);    
                match.payload.status = Status::FILLED; 
            } else level.reduce_shares(filled_qty);

            Quote q = generate_quote_(
                ticker_, fill_price, orderbook_.best_bid(), orderbook_.best_ask(), volume_
            );
            
            Message<Trade> t = generate_trade_(
                msg.payload, match.payload, filled_qty, fill_price
            );

        } 
        ++it; // move to next price
    } 
}


void exchange::MatchingEngine::match_FOK_buy_(Message<exchange::Order>& msg) {

    if (orderbook_.check_ask_shares_limit_ge(msg.payload.qty, msg.payload.price) == 0) 
        return; // TODO MARK incoming order as canceled and inform client on the broker side 

    auto it{ orderbook_.asks().begin() };
    // it->first = price
    // it->second = PriceLevel&

    while (it != orderbook_.asks().end() && msg.payload.qty > 0) { 
        if (it->first < msg.payload.price) {  
            return;
        }

        while(!it->second.empty() && msg.payload.qty > 0) {

            Message<exchange::Order>& match = it->second.front(); // first order at this price 
            PriceLevel& level = it->second;

            if (msg.payload.status == Status::CANCELED) { // if order dead (canceled)
                // should be renamed to canceled since cancel order already had their qty removed from the orderbook so it makes more sense we are just dequeueing
                level.consume_front(0);
                continue;
            }

            const uint64_t filled_qty{ std::min(msg.payload.qty, match.payload.qty) };
            const uint64_t fill_price{ match.payload.price };

            volume_ += filled_qty;
            match.payload.qty -= filled_qty;
            msg.payload.qty -= filled_qty;

            if (match.payload.qty == 0) { 
                orderbook_.remove_order_ptr(match.payload.oid);
                level.consume_front(filled_qty);    
                match.payload.status = Status::FILLED; 
            } else level.reduce_shares(filled_qty);

            Quote q = generate_quote_(
                ticker_, fill_price, orderbook_.best_bid(), orderbook_.best_ask(), volume_
            );
            
            Message<Trade> t = generate_trade_(
                msg.payload, match.payload, filled_qty, fill_price
            );

        } 
        ++it; // move to next price
    } 
}


void exchange::MatchingEngine::match_FOK_sell_(Message<exchange::Order>& msg) {

    if (orderbook_.check_bid_shares_limit_ge(msg.payload.qty, msg.payload.price) == 0) 
        return; // TODO MARK incoming order as canceled and inform client on the broker side 

    auto it{ orderbook_.bids().begin() };
    // it->first = price
    // it->second = PriceLevel&

    while (it != orderbook_.bids().end() && msg.payload.qty > 0) { 
        if (it->first < msg.payload.price) {  
            return;
        }

        while(!it->second.empty() && msg.payload.qty > 0) {

            Message<exchange::Order>& match = it->second.front(); // first order at this price 
            PriceLevel& level = it->second;

            if (msg.payload.status == Status::CANCELED) { // if order dead (canceled)
                // should be renamed to canceled since cancel order already had their qty removed from the orderbook so it makes more sense we are just dequeueing
                level.consume_front(0);
                continue;
            }

            const uint64_t filled_qty{ std::min(msg.payload.qty, match.payload.qty) };
            const uint64_t fill_price{ match.payload.price };

            volume_ += filled_qty;
            match.payload.qty -= filled_qty;
            msg.payload.qty -= filled_qty;

            if (match.payload.qty == 0) { 
                orderbook_.remove_order_ptr(match.payload.oid);
                level.consume_front(filled_qty);    
                match.payload.status = Status::FILLED; 
            } else level.reduce_shares(filled_qty);

            Quote q = generate_quote_(
                ticker_, fill_price, orderbook_.best_bid(), orderbook_.best_ask(), volume_
            );
            
            Message<Trade> t = generate_trade_(
                msg.payload, match.payload, filled_qty, fill_price
            );

        } 
        ++it; // move to next price
    } 
}