#ifndef BOOK_H
#define BOOK_H

#include "price_level.h"

struct Book {
    public:
        Book(int size,int peg_price) 
            : peg_price(peg_price), lower_bound(peg_price - (size / 2)), upper_bound(peg_price + (size / 2)) {
                for (int i = 0; i < size; ++i){
                    price_levels.emplace_back();
                }
        };
        ~Book() = default;
        uint32_t lower_bound;
        uint32_t upper_bound;
        uint32_t peg_price;
        std::vector<PriceLevel> price_levels;
    private:
        bool is_buy;
        uint64_t total_volume;
};

#endif