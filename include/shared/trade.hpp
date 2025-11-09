#pragma once
#include <cstdint>
#include <string>


namespace exchange {

    struct Trade {
        Trade() = default;
        
        // Sequence
        uint64_t trade_id;

        // Client details
        uint64_t buy_order_id;
        uint64_t sell_order_id;
        uint64_t buy_client_id;
        uint64_t sell_client_id;
  

        // Order details
        std::string ticker;
        uint64_t price;      // Price is never negative
        uint64_t quantity;   // Quantity is never negative 
        uint64_t remaining;  // Partial fill

        // Timing
        uint64_t timestamp;
        
        Trade(uint64_t trade_id_,
          uint64_t buy_order_id_,
          uint64_t sell_order_id_,
          uint64_t buy_client_id_,
          uint64_t sell_client_id_,
          std::string ticker_,
          uint64_t price_,
          uint64_t quantity_,
          uint64_t remaining_,
          uint64_t timestamp_
        )
        : trade_id(trade_id_),
          buy_order_id(buy_order_id_),
          sell_order_id(sell_order_id_),
          buy_client_id(buy_client_id_),
          sell_client_id(sell_client_id_),
          ticker(std::move(ticker_)),
          price(price_),
          quantity(quantity_),
          remaining(remaining_),
          timestamp(timestamp_)
        {}
    };

}
