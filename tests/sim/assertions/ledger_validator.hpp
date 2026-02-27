#pragma once

#include "shared/trade.hpp"
#include <vector>
#include <string>

namespace sim {

// Validate a collection of trades produced during a simulation.
// Returns empty string on success, or a description of the first violation.

std::string validate_trades_positive(const std::vector<Trade>& trades);
std::string validate_trades_no_self_trade(const std::vector<Trade>& trades);
std::string validate_trades_conservation(const std::vector<Trade>& trades,
                                         uint64_t expected_total_qty);

} // namespace sim
