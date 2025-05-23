#ifndef MARKET_H
#define MARKET_H

#include <queue>

#include "book.h"
#include "order.h"





struct Market {
    public:
        Market();
        void add_order(Order& order);
        void simulate();
        uint64_t get_buy_volume() const;
        uint64_t get_sell_volume() const;
        uint64_t get_buy_price() const;
        uint64_t get_sell_price() const;
        uint64_t get_order_count() const;
    private:
        std::queue<Order*> order_queue;
        Book buy_book;
        Book sell_book; 
        uint64_t orders;   
        uint64_t users;
};



#endif