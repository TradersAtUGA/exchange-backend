#include "../../pch.h"

#include "matching-engine/matching_engine.hpp"
#include <algorithm> // think we can remove this 
#include <tuple>

#include "shared/utilities.hpp"

#include <iostream> // DEBUG 

void exchange::MatchingEngine::process(exchange::Order& order) { 
    mkt_to_lim_(order); // conv mkt -> lim 
    (this->*MATCH_FUNC_LUT
        [side_lut_converter_(order.side)]
        [order_type_lut_converter_(order.order_type)]
        [tif_lut_converter_(order.tif)]
    )(order);
}

Trade exchange::MatchingEngine::create_trade_(
    const exchange::Order& bid_order, 
    const exchange::Order& ask_order, 
    uint64_t filled_qty, 
    uint64_t trade_price) {

    return Trade(
        bid_order.cid,
        ask_order.cid,
        static_cast<uint64_t>(1), // this is the trade id we will need some static increment variable
        filled_qty,
        exchange::uint64_t_to_double(trade_price), // price was originally bitcasted 
        static_cast<int8_t>(1), // trade obj return code; check docs for more info
        bid_order.ticker
    );
}

void exchange::MatchingEngine::match_IOC_buy_(exchange::Order& order) { 
    
    auto it{ orderbook_.asks().begin() };
    // it->first = price
    // it->second = PriceLevel&

    while (it != orderbook_.asks().end() && order.qty > 0) { 
        if (it->first > order.price) {  
            return; // no push to orderbook 
        }

        while(!it->second.empty() && order.qty > 0) {

            exchange::Order& match = it->second.front(); // first order at this price 
            PriceLevel& level = it->second;

            if (match.status == 0) { // if order dead
                level.consume_front(0);
                continue;
            }

            const uint64_t filled_qty{ std::min(order.qty, match.qty) };
            const uint64_t fill_price{ match.price };

            ledger_.emplace_back(
                create_trade_(order, match, filled_qty, fill_price)
            );

            match.qty -= filled_qty;
            order.qty -= filled_qty;

            if (match.qty == 0) { 
                match.status = 0; // set  order to dead since match complete 
                orderbook_.remove_order_ptr(match.oid); 
                level.consume_front(filled_qty); // if this first order is empty
            }
        } 
        ++it; // move to next price
    } 
}

void exchange::MatchingEngine::match_FOK_buy_(exchange::Order& order) { 

    if (orderbook_.check_ask_shares_limit_ge(order.qty, order.price) == 0) 
        return; // need to find a way to reject order here...
    
    auto it{ orderbook_.asks().begin() };
    // it->first = price
    // it->second = PriceLevel&

    while (it != orderbook_.asks().end() && order.qty > 0) { 
        if (it->first > order.price) {  
            return; // no push to orderbook 
        }

        while(!it->second.empty() && order.qty > 0) {

            exchange::Order& match = it->second.front(); // first order at this price 
            PriceLevel& level = it->second;

            if (match.status == 0) { // if order dead
                level.consume_front(0);
                continue;
            }

            const uint64_t filled_qty{ std::min(order.qty, match.qty) };
            const uint64_t fill_price{ match.price };

            ledger_.emplace_back(
                create_trade_(order, match, filled_qty, fill_price)
            );

            match.qty -= filled_qty;
            order.qty -= filled_qty;

            if (match.qty == 0) { 
                match.status = 0; // set  order to dead since match complete 
                orderbook_.remove_order_ptr(match.oid); 
                level.consume_front(filled_qty); // if this first order is empty
            }
        } 
        ++it; // move to next price
    } 
}

void exchange::MatchingEngine::match_IOC_sell_(exchange::Order& order) { 
    
    auto it{ orderbook_.bids().begin() };
    // it->first = price
    // it->second = PriceLevel&

    while (it != orderbook_.bids().end() && order.qty > 0) { 
        if (it->first < order.price) {  
            return; // no push to orderbook 
        }

        while(!it->second.empty() && order.qty > 0) {

            exchange::Order& match = it->second.front(); // first order at this price 
            PriceLevel& level = it->second;

            if (match.status == 0) { // if order dead
                level.consume_front(0);
                continue;
            }

            const uint64_t filled_qty{ std::min(order.qty, match.qty) };
            const uint64_t fill_price{ match.price };

            ledger_.emplace_back(
                create_trade_(order, match, filled_qty, fill_price)
            );

            match.qty -= filled_qty;
            order.qty -= filled_qty;

            if (match.qty == 0) { 
                match.status = 0; // set  order to dead since match complete 
                orderbook_.remove_order_ptr(match.oid); 
                level.consume_front(filled_qty); // if this first order is empty
            }
        } 
        ++it; // move to next price
    } 
}

void exchange::MatchingEngine::match_FOK_sell_(exchange::Order& order) { 

    if (orderbook_.check_bid_shares_limit_ge(order.qty, order.price) == 0) 
        return; // need to find a way to reject order here...
    
    auto it{ orderbook_.bids().begin() };
    // it->first = price
    // it->second = PriceLevel&

    while (it != orderbook_.bids().end() && order.qty > 0) { 
        if (it->first < order.price) {  
            return; // no push to orderbook 
        }

        while(!it->second.empty() && order.qty > 0) {

            exchange::Order& match = it->second.front(); // first order at this price 
            PriceLevel& level = it->second;

            if (match.status == 0) { // if order dead
                level.consume_front(0);
                continue;
            }

            const uint64_t filled_qty{ std::min(order.qty, match.qty) };
            const uint64_t fill_price{ match.price };

            ledger_.emplace_back(
                create_trade_(order, match, filled_qty, fill_price)
            );

            match.qty -= filled_qty;
            order.qty -= filled_qty;

            if (match.qty == 0) { 
                match.status = 0; // set  order to dead since match complete 
                orderbook_.remove_order_ptr(match.oid); 

                // TODO: we need to be able to remove orders without removing in place since the line above when we reduce the order obj it self does not modify the pricelevel share count just create another method 
                level.consume_front(filled_qty); // if this first order is empty
            }
        } 
        ++it; // move to next price
    } 
}

void exchange::MatchingEngine::match_DAY_buy_(exchange::Order& order) {

    auto it{ orderbook_.asks().begin() };
    // it->first = price
    // it->second = PriceLevel&

    while (it != orderbook_.asks().end() && order.qty > 0) { 
        if (it->first > order.price && order.order_type != OrderType::MARKET) {  
            // add what ever is left on the order
            // useful for init of orderbook if this is the first order coming in 
            on_partial_fill_aggressive_limit_(order);
            return;
        }

        while(!it->second.empty() && order.qty > 0) {

            exchange::Order& match = it->second.front(); // first order at this price 
            PriceLevel& level = it->second;

            if (match.status == 0) { // if order dead
                level.consume_front(0);
                continue;
            }

            const uint64_t filled_qty{ std::min(order.qty, match.qty) };
            const uint64_t fill_price{ match.price };

            ledger_.emplace_back(
                create_trade_(order, match, filled_qty, fill_price)
            );

            match.qty -= filled_qty;
            order.qty -= filled_qty;

            match.status = 0; // set  order to dead since match complete 
            orderbook_.remove_order_ptr(match.oid);

            if (match.qty == 0) level.consume_front(filled_qty); // if this first order is empty
        } 

        ++it; // move to next price
    } 
    if (order.order_type != OrderType::MARKET) on_partial_fill_aggressive_limit_(order);
}

void exchange::MatchingEngine::match_DAY_sell_(exchange::Order& order) {

    auto it{ orderbook_.bids().begin() };
    // it->first = price
    // it->second = PriceLevel&

    while (it != orderbook_.bids().end() && order.qty > 0) { 
        if (it->first < order.price && order.order_type != OrderType::MARKET) {  
            // add what ever is left on the order
            // useful for init of orderbook if this is the first order coming in 
            on_partial_fill_aggressive_limit_(order);
            return;
        }

        while(!it->second.empty() && order.qty > 0) {

            exchange::Order& match = it->second.front(); // first order at this price 
            PriceLevel& level = it->second;

            if (match.status == 0) { // if order dead
                level.consume_front(0);
                continue;
            }

            const uint64_t filled_qty{ std::min(order.qty, match.qty) };
            const uint64_t fill_price{ match.price };

            ledger_.emplace_back(
                create_trade_(order, match, filled_qty, fill_price)
            );

            match.qty -= filled_qty;
            order.qty -= filled_qty;

            match.status = 0; // set  order to dead since match complete 
            orderbook_.remove_order_ptr(match.oid);

            if (match.qty == 0) level.consume_front(filled_qty); // if this first order is empty
        } 

        ++it; // move to next price
    } 
    if (order.order_type != OrderType::MARKET) on_partial_fill_aggressive_limit_(order);
}
