#ifndef BOOK_H
#define BOOK_H

#include <cstdint>

struct Book {
    public:
        Book(int size, bool is_buy);
        ~Book();
    private:
        int get_index(uint32_t price) const;
        bool is_buy;
        uint64_t total_volume;
        int best_price;
};

#endif