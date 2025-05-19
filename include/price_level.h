#ifndef PRICE_LEVEL_H
#define PRICE_LEVEL_H
#include "order.h"
#include <vector>
#include <cstdint>


struct PriceLevel {
    std::vector<std::pair<uint32_t, uint32_t>> orders;

    PriceLevel() = default;
    ~PriceLevel() = default;

};
#endif