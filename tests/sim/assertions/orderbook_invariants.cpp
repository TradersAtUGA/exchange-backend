#include "assertions/orderbook_invariants.hpp"
#include <limits>
#include <sstream>

namespace sim {

std::string check_no_crossed_book(const exchange::OrderBook& ob) {
    uint64_t best_bid = ob.best_bid();
    uint64_t best_ask = ob.best_ask();

    // Ignore sentinel values
    if (best_bid == 0 || best_ask == std::numeric_limits<uint64_t>::max())
        return "";

    if (best_bid >= best_ask) {
        std::ostringstream ss;
        ss << "Crossed book: best_bid=" << best_bid << " >= best_ask=" << best_ask;
        return ss.str();
    }
    return "";
}

std::string check_sentinel_integrity(const exchange::OrderBook& ob) {
    // Bid side should have price 0 sentinel
    if (ob.bids().find(0) == ob.bids().end())
        return "Missing bid sentinel at price 0";

    // Ask side should have UINT64_MAX sentinel
    if (ob.asks().find(std::numeric_limits<uint64_t>::max()) == ob.asks().end())
        return "Missing ask sentinel at price UINT64_MAX";

    return "";
}

std::string check_no_zero_qty_orders(const exchange::OrderBook& ob) {
    // Check bid price levels
    for (const auto& [price, level] : ob.bids()) {
        if (price == 0) continue; // sentinel
        if (level.order_count() > 0 && level.total_shares() == 0) {
            std::ostringstream ss;
            ss << "Bid price level " << price << " has "
               << level.order_count() << " orders but 0 total shares";
            return ss.str();
        }
    }
    // Check ask price levels
    for (const auto& [price, level] : ob.asks()) {
        if (price == std::numeric_limits<uint64_t>::max()) continue; // sentinel
        if (level.order_count() > 0 && level.total_shares() == 0) {
            std::ostringstream ss;
            ss << "Ask price level " << price << " has "
               << level.order_count() << " orders but 0 total shares";
            return ss.str();
        }
    }
    return "";
}

std::string check_share_count_consistency(const exchange::OrderBook& ob) {
    // We can't easily iterate orders inside each PriceLevel ring buffer
    // from the public API, but we can check that total_shares >= 0
    // and order_count is consistent with empty()
    for (const auto& [price, level] : ob.bids()) {
        if (price == 0) continue;
        if (level.empty() && level.order_count() != 0) {
            std::ostringstream ss;
            ss << "Bid level " << price << " is empty() but order_count=" << level.order_count();
            return ss.str();
        }
    }
    for (const auto& [price, level] : ob.asks()) {
        if (price == std::numeric_limits<uint64_t>::max()) continue;
        if (level.empty() && level.order_count() != 0) {
            std::ostringstream ss;
            ss << "Ask level " << price << " is empty() but order_count=" << level.order_count();
            return ss.str();
        }
    }
    return "";
}

std::string check_orderbook_invariants(const exchange::OrderBook& ob) {
    std::string result;

    result = check_no_crossed_book(ob);
    if (!result.empty()) return result;

    result = check_sentinel_integrity(ob);
    if (!result.empty()) return result;

    result = check_no_zero_qty_orders(ob);
    if (!result.empty()) return result;

    result = check_share_count_consistency(ob);
    if (!result.empty()) return result;

    return "";
}

} // namespace sim
