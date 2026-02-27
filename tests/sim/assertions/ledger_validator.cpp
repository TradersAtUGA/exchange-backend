#include "assertions/ledger_validator.hpp"
#include <sstream>
#include <numeric>

namespace sim {

std::string validate_trades_positive(const std::vector<Trade>& trades) {
    for (size_t i = 0; i < trades.size(); ++i) {
        if (trades[i].qty == 0) {
            std::ostringstream ss;
            ss << "Trade " << i << " has qty=0";
            return ss.str();
        }
    }
    return "";
}

std::string validate_trades_no_self_trade(const std::vector<Trade>& trades) {
    for (size_t i = 0; i < trades.size(); ++i) {
        if (trades[i].bcid == trades[i].acid && trades[i].bcid != 0) {
            std::ostringstream ss;
            ss << "Trade " << i << " is a self-trade: bcid=acid=" << trades[i].bcid;
            return ss.str();
        }
    }
    return "";
}

std::string validate_trades_conservation(const std::vector<Trade>& trades,
                                         uint64_t expected_total_qty) {
    uint64_t total = 0;
    for (const auto& t : trades) {
        total += t.qty;
    }
    if (total != expected_total_qty) {
        std::ostringstream ss;
        ss << "Trade quantity mismatch: total=" << total
           << " expected=" << expected_total_qty;
        return ss.str();
    }
    return "";
}

} // namespace sim
