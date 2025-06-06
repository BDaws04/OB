#ifndef PRICE_LEVEL_H
#define PRICE_LEVEL_H

#include <vector>
#include <cstdint>
#include <iostream>

struct PriceLevel {
    public:
        PriceLevel() : total_volume_(0) {}

        void add_order(uint32_t order_id, uint32_t volume) {

            orders.push_back({order_id, volume});
            total_volume_ += volume;
        }

        void remove_volume(uint32_t volume){

            if (volume == total_volume_) {
                orders.clear();
                total_volume_ = 0;
                return;
            }

            if (volume > total_volume_) {
                    volume = total_volume_; 
            }

            int n = orders.size();
            total_volume_ -= volume;

            int new_start = 0;

            for (int i = 0; i < n; ++i){
                if (volume < orders[i].second) {
                    orders[i].second -= volume;
                    break;
                } else {
                    volume -= orders[i].second;
                    orders[i].second = 0;
                    new_start++;
                }
            }
            orders.erase(orders.begin(), orders.begin() + new_start);
        };

        inline uint32_t get_total_volume() const { return total_volume_; }


    private:
        uint32_t total_volume_;
        // <order_id, volume>
        std::vector<std::pair<uint32_t, uint32_t>> orders;

};
#endif