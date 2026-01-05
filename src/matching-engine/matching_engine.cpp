#include "matching-engine/matching_engine.hpp"
#include <algorithm>

#include <iostream> // DEBUG 

// main entry point -- moves order by TIF, 
// each TIF handle moves by market or limit 
// then each of those have handles 
void exchange::MatchingEngine::process(exchange::Order& order) {
    if (order.tif == TIF::DAY) {
        handle_DAY_(order);
    } else if (order.tif == TIF::FOK) { 
        handle_FOK_(order);
    } else if (order.tif == TIF::IOC) {
        handle_IOC_(order);
    }
}

void exchange::MatchingEngine::handle_DAY_(exchange::Order& order) {
    if (order.order_type == OrderType::LIMIT) { 
        handle_LMT_(order);
    } else { // exchange::OrderType::MARKET
        handle_MKT_(order);
    }
}

void exchange::MatchingEngine::handle_FOK_(exchange::Order& order) {
    if (order.order_type == OrderType::LIMIT) {
        
        if (order.side == Side::BUY) {
            // we should prob rename these orderbook methods 
            // to match the side enums
            uint64_t avail = orderbook_.check_ask_shares_limit_ge(
                order.qty, order.price
            );

            if (avail > order.qty) {
                // treat as regular limit atp 
                // guaranteed to fill since we checked before
                match_LMT_buy_no_push_(order); 
            } else {
                order.status = 5; // see docs/order.md
            }
        } else { // Side::SELL
            uint64_t avail = orderbook_.check_bid_shares_limit_ge(
                order.qty, order.price
            );

            if (avail > order.qty) {
                // treat as regular limit atp 
                // guaranteed to fill since we checked before
                match_LMT_sell_no_push_(order); 
            } else {
                order.status = 5; // see docs/order.md
            }
        }
    } else { // exchange::OrderType::MARKET
        
        if (order.side == Side::BUY) {
            uint64_t avail = orderbook_.check_ask_shares_limit_ge(
                order.qty, order.price
            );

            if (avail > order.qty) {
                match_MKT_buy_(order); 
            } else {
                order.status = 5; // see docs/order.md
            }
        } else { // Side::SELL
            uint64_t avail = orderbook_.check_bid_shares_limit_ge(
                order.qty, order.price
            );

            if (avail > order.qty) {
                match_MKT_sell_(order); 
            } else {
                order.status = 5; // see docs/order.md
            }
        }
    }
}

void exchange::MatchingEngine::handle_IOC_(exchange::Order& order) { 
    if (order.order_type == OrderType::LIMIT) {
        if (order.side == Side::BUY) {
            match_LMT_buy_no_push_(order);
        } else { // Side:SELL
            match_LMT_sell_no_push_(order);
        }   
    } else { // exchange::OrderType::MARKET
        // MARKET IOC ~ MARKET DAY  
        handle_MKT_(order);
    }
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
        trade_price,
        static_cast<int8_t>(1), // trade obj return code; check docs for more info
        bid_order.ticker
    );
}


void exchange::MatchingEngine::handle_LMT_(exchange::Order& order) {
    if (order.side == Side::BUY) {
        match_LMT_buy_(order);
    } else { // Side::SELL
        match_LMT_sell_(order);
    }
}

void exchange::MatchingEngine::match_LMT_buy_(exchange::Order& order) {

    auto it{ orderbook_.asks().begin() };
    // it->first = price
    // it->second = PriceLevel&


    while (it != orderbook_.asks().end() && order.qty > 0) { 
        if (it->first > order.price) {  
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

    // if partial fill occurred
    on_partial_fill_aggressive_limit_(order);
}

void exchange::MatchingEngine::match_LMT_sell_(exchange::Order& order) {

    auto it{ orderbook_.bids().begin() };

    while (it != orderbook_.bids().end() && order.qty > 0) { 
        if (it->first < order.price) {
            on_partial_fill_aggressive_limit_(order);
            return;
        }

        while(!it->second.empty() && order.qty > 0) {

            exchange::Order& match = it->second.front();
            PriceLevel& level = it->second;

            if (match.status == 0) { // if order dead
                level.consume_front(0);
                continue;
            }

            uint64_t filled_qty{ std::min(order.qty, match.qty) };

            uint64_t fill_price{ match.price };

            ledger_.emplace_back(
                create_trade_(order, match, filled_qty, fill_price)
            );

            match.qty -= filled_qty;
            order.qty -= filled_qty;

            if (match.qty == 0) level.consume_front(filled_qty);
        } 

        ++it;
    } 

    // if partial fill occurred
    on_partial_fill_aggressive_limit_(order);
}

void exchange::MatchingEngine::on_partial_fill_aggressive_limit_(exchange::Order& order) {
    if (order.qty == 0) return;
    orderbook_.add_order(order);
}


void exchange::MatchingEngine::handle_MKT_(exchange::Order& order) {
    if (order.side == Side::BUY) {
        match_MKT_buy_(order);
    } else { // Side::SELL
        match_MKT_sell_(order);
    }
}

void exchange::MatchingEngine::match_MKT_buy_(exchange::Order& order) {

    auto it{ orderbook_.asks().begin() };

    while(it != orderbook_.asks().end() && order.qty > 0) {

        while (!it->second.empty() && order.qty > 0) {

            PriceLevel& level = it->second;
            exchange::Order& match = level.front();

            if (match.status == 0) {
                // delete ordered already remove share count
                level.consume_front(0); 
            }

            const uint64_t filled_qty{ std::min(order.qty, match.qty) };
            const uint64_t fill_price{ match.price };
            ledger_.emplace_back(
                create_trade_(order, match, filled_qty, fill_price)
            );

            match.qty -= filled_qty;
            order.qty -= filled_qty;

            if (match.qty == 0) level.consume_front(filled_qty);
        } 
        ++it;
    }   
}

void exchange::MatchingEngine::match_MKT_sell_(exchange::Order& order) {

    auto it{ orderbook_.bids().begin() };

    while(it != orderbook_.bids().end() && order.qty > 0) {

        while (!it->second.empty() && order.qty > 0) {

            PriceLevel& level = it->second;
            exchange::Order& match = level.front();

            if (match.status == 0) {
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
            
            if (match.qty == 0) level.consume_front(filled_qty);
        }
        ++it;
    } 
}

void exchange::MatchingEngine::match_LMT_buy_no_push_(exchange::Order& order) {
    
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

            match.status = 0; // set  order to dead since match complete 
            orderbook_.remove_order_ptr(match.oid);

            if (match.qty == 0) level.consume_front(filled_qty); // if this first order is empty
        } 

        ++it; // move to next price
    } 
} 

void exchange::MatchingEngine::match_LMT_sell_no_push_(exchange::Order& order) { 
    auto it{ orderbook_.bids().begin() };
    // it->first = price
    // it->second = PriceLevel&

    while (it != orderbook_.bids().end() && order.qty > 0) { 
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

            match.status = 0; // set  order to dead since match complete 
            orderbook_.remove_order_ptr(match.oid);

            if (match.qty == 0) level.consume_front(filled_qty); // if this first order is empty
        } 
        ++it; // move to next price
    } 
}