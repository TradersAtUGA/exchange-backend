#pragma once 

#include <cstdint>

struct Quote {
    uint64_t price; // last price 
    uint64_t bid;  // best bidding price 
    uint64_t ask; // best asking price 
    uint64_t volume; // total volume since open (cumulative)
};