#ifndef MARKET_H
#define MARKET_H

#include "book.h"
#include "order.h"
#include "order_queue.h"

#include <iostream>
#include <windows.h>


struct Market {
    public:
        Market(uint32_t peg_price, int levels = 251)
            : peg_price(peg_price),
              buy_book(levels, peg_price),
              sell_book(levels, peg_price),
              order_count(0),
              user_count(0), 
              success_orders(0),
              failed_orders(0) {};

        inline int get_index(uint32_t price) const {
            if (price < buy_book.lower_bound || price > buy_book.upper_bound) {
                return -1; 
            }
            return price - buy_book.lower_bound;
        };

        inline uint64_t get_success_orders() const {
            return success_orders;
        };

        inline uint64_t get_failed_orders() const {
            return failed_orders;
        };

        inline uint32_t get_lower_bound() const {
            return buy_book.lower_bound;
        };

        inline uint32_t get_upper_bound() const {
            return buy_book.upper_bound;
        };

        void add_order(Order order){
            orders.push(order);
        };

        void simulate() {
            HANDLE this_thread = GetCurrentThread();
            DWORD_PTR mask = 1 << 0;
            if (SetThreadAffinityMask(this_thread, mask) == 0) {
                std::cerr << "Failed to set thread affinity inside simulate()" << std::endl;
            }
            while (running) {
                Order current = orders.pop();
                current.id = order_count++;
                
                if (current.is_GTC()) {
                    place_gtc_order(current);
                }
                else if (current.is_IOC()) {
                    place_ioc_order(current);
                }
                else if (current.is_FOK()) {
                    place_fok_order(current);
                }
            }

            while(!orders.empty()) {
                Order current = orders.pop();
                current.id = order_count++;
                
                if (current.is_GTC()) {
                    place_gtc_order(current);
                }
                else if (current.is_IOC()) {
                    place_ioc_order(current);
                }
                else if (current.is_FOK()) {
                    place_fok_order(current);
                }
            }
        }

        void print_buy_book() const {
            for (int i = 0; i < buy_book.price_levels.size(); ++i) {
                if (buy_book.price_levels[i].get_total_volume() > 0) {
                    std::cout << "Buy Price: " << (buy_book.lower_bound + i) << ", Volume: " << buy_book.price_levels[i].get_total_volume() << std::endl;
                }
            }
        };

        void stop(){
            running = false;
        }

        void print_sell_book() const {
            for (int i = 0; i < sell_book.price_levels.size(); ++i) {
                if (sell_book.price_levels[i].get_total_volume() > 0) {
                    std::cout << "Sell Price: " << (sell_book.lower_bound + i) << ", Volume: " << sell_book.price_levels[i].get_total_volume() << std::endl;
                }
            }
        };


        inline uint64_t get_order_count() const {
            return order_count;
        };

        void place_fok_order(Order& order){
            int index = get_index(order.price());
            if (index < 0) {
                return;
            }
            if (order.is_buy()) {
                if (sell_book.price_levels[index].get_total_volume() <= order.volume()) {
                    sell_book.price_levels[index].remove_volume(order.volume());
                    success_orders++;
                } else {
                    failed_orders++;
                    return;
                }

            } else {
                if (buy_book.price_levels[index].get_total_volume() <= order.volume()) {
                    buy_book.price_levels[index].remove_volume(order.volume());
                    success_orders++;
                } else {
                    failed_orders++;
                    return;
                }
            }
        };
        void place_ioc_order(Order& order){
            int index = get_index(order.price());
            if (index < 0) {
                return;
            }
            if (order.is_buy()) {
                if (sell_book.price_levels[index].get_total_volume() > 0) {

                    int removed_volume = std::min(order.volume(), sell_book.price_levels[index].get_total_volume());
                    sell_book.price_levels[index].remove_volume(removed_volume);
                    if (removed_volume != order.volume()) {
                        buy_book.price_levels[index].add_order(order.id, order.volume() - removed_volume);
                    }
                    success_orders++;
                } else {
                    failed_orders++;
                    return;
                }
            } else {
                if (buy_book.price_levels[index].get_total_volume() > 0) {
                    int removed_volume = std::min(order.volume(), buy_book.price_levels[index].get_total_volume());
                    buy_book.price_levels[index].remove_volume(removed_volume);
                    if (removed_volume != order.volume()) {
                        sell_book.price_levels[index].add_order(order.id, order.volume() - removed_volume);
                    }
                    success_orders++;
                } else {
                    failed_orders++;
                    return;
                }
            }
        };

        void place_gtc_order(Order& order){
            int index = get_index(order.price());
            if (index < 0) {
                return;
            }
            if (order.is_buy()) {
                buy_book.price_levels[index].add_order(order.id, order.volume());
            } else {
                sell_book.price_levels[index].add_order(order.id, order.volume());
            }
            success_orders++;
        };

    private:
        OrderQueue<Order> orders;
        Book buy_book;
        Book sell_book; 
        uint64_t order_count;   
        uint64_t user_count;
        uint32_t peg_price;
        uint64_t success_orders;
        uint64_t failed_orders;
        bool running = true; 
};


#endif