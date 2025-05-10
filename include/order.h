#ifndef ORDER_H
#define ORDER_H

#include <cstdint>

struct alignas(16) Order {
    public:
        uint32_t id;
        uint32_t price;
        uint32_t volume;
        uint32_t metadata; 
};

#endif