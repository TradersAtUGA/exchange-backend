#pragma once 

#include <cstdint>
#include <string>
#include <ostream>
#include <nlohmann/json.hpp>

#include "shared/utilities.hpp"

using json = nlohmann::json;

struct Quote {

    std::string to_json() {
        json j; 
        j["ticker"] = ticker;
        j["price"] = price; 
        j["bid"] = bid; 
        j["ask"] = ask;
        j["volume"] = volume;
        return j.dump(); 
    }

    friend std::ostream& operator<<(std::ostream& os, const Quote& q) {
        os << "ticker: " << q.ticker << "\n";
        os << "price: " << q.price << "\n";
        os << "bid price: " << q.bid << "\n";
        os << "ask price: " << q.ask << "\n";
        os << "volume: " << q.volume << "\n";
        return os; 
    }

    std::string ticker;
    uint64_t price; // last price 
    uint64_t bid;  // best bidding price 
    uint64_t ask; // best asking price 
    uint64_t volume; // total volume since open (cumulative)
};