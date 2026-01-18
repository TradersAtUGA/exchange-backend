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



namespace exchange {

class MatchingEngine {
public:

   
    explicit MatchingEngine(std::string ticker) : 
        last_price_(0),
        ledger_({}),
        orders_(),
        orderbook_(ticker) {}

    /** 
     * @brief entry point for all orders entering the matching engine
     *  
     * @param order the order to process inside the matching engine
     */
    void process(exchange::Order& order);

    /**
     * @brief cancels an order that was already placed
     * 
     * @param order the order to cancel
     */
    void cancel(exchange::Order& order) {orderbook_.cancel_order(order); }

    /** @brief returns the current best asking price */
    uint64_t ask() const { return orderbook_.best_ask(); }

    /** @brief returns the current best bidding price */
    uint64_t bid() const { return orderbook_.best_bid(); }

    /** @brief returns the last market price */ 
    uint64_t last() const noexcept { return last_price_; }

    /** @brief returns the ledger containing all previous confirmed trades */
    const std::deque<Trade>& ledger() const { return ledger_; } 

    /** @brief returns the orderbook */
    const exchange::OrderBook& orderbook() const { return orderbook_; }

    /** @brief returns the inbound ring buffer */
    const OrderbookRingBuffer<exchange::Order, config::RING_BUFFER_SIZE>& orders() const { return orders_; }

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

    // MATCHING ALGOS
    /** @brief matching algo for an buy side IOC order */
    void match_IOC_buy_(exchange::Order& order); 

    /** @brief matching algo for an sell side IOC order */
    void match_IOC_sell_(exchange::Order& order);

    /** @brief matching algo for an buy side FOK order */
    void match_FOK_buy_(exchange::Order& order);

    /** @brief matching algo for an sell side FOK order */
    void match_FOK_sell_(exchange::Order& order);

    /** @brief matching algo for an buy side DAY order */
    void match_DAY_buy_(exchange::Order& order);

    /** @brief matching algo for an sell side DAY order */
    void match_DAY_sell_(exchange::Order& order);

    // HELPERS

    /** @brief adds a partially filled aggressive LMT order back to the orderbook */
    void on_partial_fill_aggressive_limit_(exchange::Order& order);

    /** @brief side enum conversion for main lookup table */
    uint8_t side_lut_converter_(Side s) noexcept {
        switch(s) {
            case Side::BUY: return 0;
            case Side::SELL: return 1;
        }
        exchange::unreachable();
    }

    /** @brief order type enum conversion for main lookup table  */
    uint8_t order_type_lut_converter_(OrderType ot) noexcept {
        switch(ot) { 
            case OrderType::MARKET: return 0;
            case OrderType::LIMIT: return 1;
        }
        exchange::unreachable();
    }

    /** @brief tif enum conversion for main lookup table */
    uint8_t tif_lut_converter_(TIF tif) noexcept { 
        switch(tif) {
            case TIF::DAY: return 0;
            case TIF::IOC: return 1; 
            case TIF::FOK: return 2; 
        }
        exchange::unreachable();
    }

    /** @brief converts a market order into a limit order  */
    void mkt_to_lim_(exchange::Order& order) noexcept {
        if (order.order_type == OrderType::MARKET) {
            // price converted to max/min depending on side
            // conventional way to limit a marketable order
            order.price = (order.side == Side::BUY) 
                ? std::numeric_limits<uint64_t>::max() : 0; 
        }
    }

    uint64_t last_price_; // last market trade price 

    // ledger might be removed and the matching engine may 
    // just use a different data structure to feed the 
    // confirmed trades out, could just be a dedicated 
    // structure to handle outbound comms...
    std::deque<Trade> ledger_; // contains transactions for matched orders
    
    OrderbookRingBuffer<exchange::Order, config::RING_BUFFER_SIZE> orders_; // incoming orders 
    exchange::OrderBook orderbook_; 
    
// forced to put public so look up table can see private method prototypes
public: 

    using MATCH_LUT_ =  
            std::array<
                std::array<
                    std::array<
                        void(exchange::MatchingEngine::*)(exchange::Order& order), 
                    3>, // num order tifs
                2>, // num order types
            2>; // num order sides 
        

    // lookup table for matching methods 
    // avoids chopped if else(s) and handlers 

    /** 
     * @brief lookup table of function pointers to send orders to their 
     * respective matching functions, should be used with the lut_converter_
     * methods to properly access indicies. 
     */
    static constexpr MATCH_LUT_ MATCH_FUNC_LUT = [] {
            MATCH_LUT_ arr; 

            // BUY MARKET DAY 
            arr[0][0][0] = &exchange::MatchingEngine::match_DAY_buy_;

            // BUY MARKET IOC
            arr[0][0][1] = &exchange::MatchingEngine::match_IOC_buy_;

            // BUY MARKET FOK
            arr[0][0][2] = &exchange::MatchingEngine::match_FOK_buy_;

            // SELL MARKET DAY 
            arr[1][0][0] = &exchange::MatchingEngine::match_DAY_sell_;

            // SELL MARKET IOC
            arr[1][0][1] = &exchange::MatchingEngine::match_IOC_sell_;

            // SELL MARKET FOK 
            arr[1][0][2] = &exchange::MatchingEngine::match_FOK_sell_;

            // BUY LIMIT DAY 
            arr[0][1][0] = &exchange::MatchingEngine::match_DAY_buy_;

            // BUY LIMIT IOC
            arr[0][1][1] = &exchange::MatchingEngine::match_IOC_buy_;

            // BUY LIMIT FOK 
            arr[0][1][2] = &exchange::MatchingEngine::match_FOK_buy_;

            // SELL LIMIT DAY 
            arr[1][1][0] = &exchange::MatchingEngine::match_DAY_sell_;

            // SELL LIMIT IOC
            arr[1][1][1] = &exchange::MatchingEngine::match_IOC_sell_;

            // SELL LIMIT FOK 
            arr[1][1][2] = &exchange::MatchingEngine::match_FOK_sell_;

            return arr; 
        }();

}; 

}
