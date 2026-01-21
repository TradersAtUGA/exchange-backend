#pragma once 

#include <cstdint>

struct Cancel {

    Cancel() : cancel_id(0), order_id(0), client_id(0) {}

    Cancel(
        uint64_t cancel_id,
        uint64_t oid, // order id 
        uint64_t cid // client id
    ) : 
        cancel_id(cancel_id),
        order_id(order_id),
        client_id(client_id) {}
    
    // number of this cancel id
    // assigned by the sequencer
    uint64_t cancel_id; 
    uint64_t order_id; // order to cancel 
    uint64_t client_id; // who is canceling the order 
};

