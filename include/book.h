#ifndef BOOK_H
#define BOOK_H

#include <cstdint>
#include <vector>
#include <price_level.h>

struct Book {
    public:
        Book(int size, bool is_buy, int peg_price) 
            : is_buy(is_buy), total_volume(0), best_price(peg_price), peg_price(peg_price), lower_bound(peg_price - (size / 2)), upper_bound(peg_price + (size / 2)) {
        };
        ~Book();
    private:
        bool is_buy;

        uint64_t total_volume;
        uint32_t best_price;
        uint32_t lower_bound;
        uint32_t upper_bound;
        uint32_t peg_price;

        std::vector<PriceLevel> price_levels;
};

#endif