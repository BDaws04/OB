#ifndef BOOK_H
#define BOOK_H

#include "price_level.h"

struct Book {
    public:
        Book(int size,int peg_price, bool is_buy) 
            : peg_price(peg_price), lower_bound(peg_price - (size / 2)), upper_bound(peg_price + (size / 2)), is_buy(is_buy) {
                for (int i = 0; i < size; ++i){
                    price_levels.emplace_back();
                }
        };
        ~Book() = default;

        void fill_market_order(uint64_t volume){
            int start_index = get_index(best_price);
            if (is_buy){
                while (volume > 0 && start_index >= 0) {
                    if (price_levels[start_index].get_total_volume() > 0) {
                        uint64_t removed_volume = price_levels[start_index].get_total_volume();
                        if (removed_volume > volume) {
                            price_levels[start_index].remove_volume(volume);
                            volume = 0;
                        } else {
                            volume -= removed_volume;
                            price_levels[start_index].remove_volume(removed_volume);
                        }
                    }
                    start_index--;
                }
                reset_best_buy_price(start_index);
            }
            else {
                while (volume > 0 && start_index < price_levels.size()) {
                    if (price_levels[start_index].get_total_volume() > 0) {
                        uint64_t removed_volume = price_levels[start_index].get_total_volume();
                        if (removed_volume > volume) {
                            price_levels[start_index].remove_volume(volume);
                            volume = 0;
                        } else {
                            volume -= removed_volume;
                            price_levels[start_index].remove_volume(removed_volume);
                        }
                    }
                    start_index++;

                }
                reset_best_sell_price(start_index);
            }
        }

        void increase_total_volume(uint64_t volume) {
            total_volume += volume;
        }
        void decrease_total_volume(uint64_t volume) {
            if (total_volume >= volume) {
                total_volume -= volume;
            } else {
                total_volume = 0;
            }
        }

        void reset_best_buy_price(int old_index){
            for (int i = old_index; i >= 0; --i) {
                if (price_levels[i].get_total_volume() > 0) {
                    best_price = lower_bound + i;
                    return;
                }
            }
            best_price = -1; 
        }

        void reset_best_sell_price(int old_index){
            for (int i = old_index; i < price_levels.size(); ++i) {
                if (price_levels[i].get_total_volume() > 0) {
                    best_price = lower_bound + i;
                    return;
                }
            }
            best_price = -1; 
        }

        inline uint64_t get_total_book_volume() const { return total_volume; }

        inline int get_index(uint32_t price) const {
            if (price < lower_bound || price > upper_bound) {
                return -1; 
            }
            return price - lower_bound;
        };

        uint32_t lower_bound;
        uint32_t upper_bound;
        uint32_t peg_price;
        std::vector<PriceLevel> price_levels;
    private:
        bool is_buy;
        uint64_t total_volume;
        uint32_t best_price = -1;
};

#endif