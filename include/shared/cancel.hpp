#pragma once 

#include <cstdint>

struct Cancel {

    Cancel();

    Cancel(
        uint64_t cancel_id,
        uint64_t order_id, 
        uint64_t client_id // client id
    ) : 
        cancel_id(cancel_id),
        oid(order_id),
        cid(client_id) {}
    

    uint64_t cancel_id; // unique id assigned by sequencer
    uint64_t oid; // order id to cancel 
    uint64_t cid; // client id 
    uint64_t coid; // client order id 
};

