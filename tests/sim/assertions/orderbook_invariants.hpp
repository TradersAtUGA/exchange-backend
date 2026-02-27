#pragma once

#include "matching-engine/orderbook.hpp"
#include <string>

namespace sim {

// Check all orderbook invariants. Returns empty string on success,
// or a description of the first violated invariant.
std::string check_orderbook_invariants(const exchange::OrderBook& ob);

// Individual invariant checks (return empty string on pass)
std::string check_no_crossed_book(const exchange::OrderBook& ob);
std::string check_sentinel_integrity(const exchange::OrderBook& ob);
std::string check_no_zero_qty_orders(const exchange::OrderBook& ob);
std::string check_share_count_consistency(const exchange::OrderBook& ob);

} // namespace sim
