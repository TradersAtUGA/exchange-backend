#pragma once 

#include <cstdint>
#include <string>
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


    std::string ticker;
    uint64_t price; // last price 
    uint64_t bid;  // best bidding price 
    uint64_t ask; // best asking price 
    uint64_t volume; // total volume since open (cumulative)
};