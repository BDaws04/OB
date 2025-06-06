#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include "order.h"
#include "market.h"

#include <memory>

struct Participant {
    public:
        inline uint32_t get_user_id() const { return user_id_; }
        inline void place_order(Order Order){
            market_->add_order(Order);
        }
        Participant(uint32_t user_id, std::shared_ptr<Market> market) : user_id_(user_id), market_(std::move(market))  {};
    private:
        uint32_t user_id_;
        std::shared_ptr<Market> market_;
};

#endif