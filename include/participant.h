#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <cstdint>
#include "order.h"
#include "market.h"

struct Participant {

    public:
        inline uint32_t get_user_id() const { return user_id_; }
        inline void place_order(Order Order){
            market_.add_order(Order);
        }
        Participant(uint32_t user_id, Market& market) : user_id_(user_id), market_(market)  {};
    private:
        uint32_t user_id_;
        Market& market_;
};

#endif