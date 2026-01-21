#pragma once 

#include <cstdint>
#include <string>
#include <deque>
#include <array>
#include <variant>

#include "shared/trade.hpp"
#include "shared/order.hpp"
#include "shared/orderbook_ringbuffer.hpp"
#include "matching-engine/orderbook.hpp"
#include "shared/utilities.hpp"
#include "shared/message.hpp"
#include "shared/quote.hpp"



namespace exchange {

class MatchingEngine {
public:

    explicit MatchingEngine(std::string ticker) : 
        last_price_(0),
        volume_(0),
        orderbook_(ticker),
        ticker_(ticker) {}

    /** 
     * @brief entry point for all orders entering the matching engine
     *  
     * @param order the order to process inside the matching engine
     */
    void process(exchange::Order& order);

    // NEW
    void process(Message<Order>& msg); 

    /**
     * @brief cancels an order that was already placed
     * 
     * @param order the order to cancel
     */
    void cancel(const exchange::Order& order) {orderbook_.cancel_order(order); }

    // NEW 
    void cancel(const Message<Cancel>& msg) { orderbook_.cancel_order(msg); } 

    /** @brief returns the current best asking price */
    uint64_t ask() const { return orderbook_.best_ask(); }

    /** @brief returns the current best bidding price */
    uint64_t bid() const { return orderbook_.best_bid(); }

    /** @brief returns the last market price */
    uint64_t last() const noexcept { return last_price_; }

    /** @brief returns the orderbook */
    const exchange::OrderBook& orderbook() const { return orderbook_; }

    /** @brief returns the total volume to this point in the matching engine */
    uint64_t volume() const noexcept { return volume_; }

private:

    //====deprecated use quickfix method for execution report 
    /**
     * @brief creates a trade object given a bid and ask order that 
     * have been matched
     * 
     * @param bid_order the order on the bidding side
     * @param ask_order the order on the asking side
     * @param filled_qty the number of shares that were filled from this trade
     * @param trade_price the price at which the trade occurred
     * @return Trade object based on given information 
     */
    Trade create_trade_(
        const exchange::Order& bid_order, 
        const exchange::Order& ask_order,
        uint64_t filled_qty, 
        uint64_t trade_price
    );

    // new handle for creating and pushing trades out to the 
    // shared mem 
    Message<Trade> generate_trade_(
        const exchange::Order& bid_order, 
        const exchange::Order& ask_order,
        uint64_t filled_qty, 
        uint64_t trade_price
    );

    /**
     *  @brief creates a new quote object to be published publicly 
     * */
    Quote generate_quote_(
        const std::string ticker,
        const uint64_t fill_price, 
        const uint64_t bid, 
        const uint64_t ask,  
        const uint64_t volume 
    ) { return {ticker, fill_price, bid, ask, volume}; }
    
    // MATCHING ALGOS

    void match_IOC_buy_(exchange::Order& order); 

    void match_IOC_sell_(exchange::Order& order);

    void match_FOK_buy_(exchange::Order& order);

    void match_FOK_sell_(exchange::Order& order);

    void match_DAY_buy_(exchange::Order& order);

    void match_DAY_sell_(exchange::Order& order);

    //

    void match_IOC_buy_(Message<exchange::Order>& order); 

    void match_IOC_sell_(Message<exchange::Order>& order);

    void match_FOK_buy_(Message<exchange::Order>& order);

    void match_FOK_sell_(Message<exchange::Order>& order);

    void match_DAY_buy_(Message<exchange::Order>& order);

    void match_DAY_sell_(Message<exchange::Order>& order);

    // HELPERS

    /** @brief adds a partially filled aggressive LMT order back to the orderbook */
    void on_partial_fill_aggressive_limit_(exchange::Order& order);

    /** @brief adds a partially filled aggressive LMT order back to the orderbook */
    void on_partial_fill_aggressive_limit_(Message<Order>& order);

    /** @brief side enum conversion for main lookup table */
    uint8_t side_lut_converter_(Side s) const noexcept {
        switch(s) {
            case Side::BUY: return 0;
            case Side::SELL: return 1;
        }
        exchange::unreachable();
    }

    /** @brief order type enum conversion for main lookup table  */
    uint8_t order_type_lut_converter_(OrderType ot) const noexcept {
        // switch(ot) { 
        //     case OrderType::MARKET: return 0;
        //     case OrderType::LIMIT: return 1;
        // }
        return 0; 
        // if more ordertypes are supported in the future 
        // then an actual conversion can be used 
    }

    /** @brief tif enum conversion for main lookup table */
    uint8_t tif_lut_converter_(TIF tif) const noexcept { 
        switch(tif) {
            case TIF::DAY: return 0;
            case TIF::IOC: return 1; 
            case TIF::FOK: return 2; 
        }
        exchange::unreachable();
    }

    void mkt_to_lim_(exchange::Order& order) const noexcept {
        switch(order.order_type) {
            case OrderType::LIMIT: return; 
            case OrderType::MARKET: (order.side == Side::BUY) 
                ? UINT64_MAX : 0; 
        }
        exchange::unreachable(); 
    }

    uint64_t last_price_; // last market trade price 
    uint64_t volume_; // monotonically increasing volume counter for the lifetime of the matching engine 
    exchange::OrderBook orderbook_; 
    std::string ticker_;

    using MATCH_LUT_ =  
            std::array<
                std::array<
                    std::array<
                        void(exchange::MatchingEngine::*)(exchange::Order& order), 
                    3>, // num order tifs
                2>, // num order types
            2>; // num order sides 
        
    using MATCH_LUT__ = 
        std::array<
                std::array<
                    std::array<
                        void(exchange::MatchingEngine::*)(Message<exchange::Order>& order), 
                    3>, // num order tifs
                2>, // num order types
            2>; // num order sides 


    // lookup table for matching methods 
    // avoids chopped if else(s) and handlers 
    static constexpr MATCH_LUT__ MATCH_FUNC_LUT = [] {
            MATCH_LUT__ arr; 

            // BUY MARKET DAY 
            arr[0][0][0] = &exchange::MatchingEngine::match_DAY_buy_;

            // BUY MARKET IOC = MARKET DAY 
            arr[0][0][1] = &exchange::MatchingEngine::match_IOC_buy_;

            // BUY MARKET FOK (uses special handle)
            arr[0][0][2] = &exchange::MatchingEngine::match_FOK_buy_;

            // SELL MARKET DAY 
            arr[1][0][0] = &exchange::MatchingEngine::match_DAY_sell_;

            // SELL MARKET IOC = MARKET DAY --------->vvvvvvvvvvvvvv
            arr[1][0][1] = &exchange::MatchingEngine::match_IOC_sell_;

            // SELL MARKET FOK (uses special handle)
            arr[1][0][2] = &exchange::MatchingEngine::match_FOK_sell_;

            // BUY LIMIT DAY 
            arr[0][1][0] = &exchange::MatchingEngine::match_DAY_buy_;

            // BUY LIMIT IOC
            arr[0][1][1] = &exchange::MatchingEngine::match_IOC_buy_;

            // BUY LIMIT FOK (uses special handle)
            arr[0][1][2] = &exchange::MatchingEngine::match_FOK_buy_;

            // SELL LIMIT DAY 
            arr[1][1][0] = &exchange::MatchingEngine::match_DAY_sell_;

            // SELL LIMIT IOC
            arr[1][1][1] = &exchange::MatchingEngine::match_IOC_sell_;

            // SELL LIMIT FOK (uses special handle)
            arr[1][1][2] = &exchange::MatchingEngine::match_FOK_sell_;

            return arr; 
        }();

}; 

}
