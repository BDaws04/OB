#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <cstdint>
#include <order.h>

struct Participant {

    public:
        inline uint32_t get_user_id() const { return user_id_; }
        inline void place_order(){}
        Participant(uint32_t user_id) : user_id_(user_id) {}
    private:
        uint32_t user_id_;
};

#endif