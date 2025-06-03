#ifndef BOOK_H
#define BOOK_H

#include <cstdint>
#include <vector>
#include <price_level.h>

struct Book {
    public:
        Book(int size,int peg_price) 
            : total_volume(0), best_price(peg_price), peg_price(peg_price), lower_bound(peg_price - (size / 2)), upper_bound(peg_price + (size / 2)) {
                for (int i = 0; i < size; ++i){
                    price_levels.emplace_back(lower_bound + i);
                }
        };
        ~Book();
        inline int get_index(uint32_t price) const {
            if (price < lower_bound || price > upper_bound) {
                return -1; 
            }
            return price - lower_bound;
        }
        uint32_t lower_bound;
        uint32_t upper_bound;
        uint32_t peg_price;
        std::vector<PriceLevel> price_levels;
    private:
        bool is_buy;

        uint64_t total_volume;
        uint32_t best_price;
};

#endif