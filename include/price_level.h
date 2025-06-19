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

    void remove_volume(uint32_t volume) {
        volume = volume > total_volume_ ? total_volume_ : volume;

        uint32_t mask = -(volume == total_volume_);

        orders.erase(orders.begin(), orders.begin() + ((mask != 0) ? orders.size() : 0));
        total_volume_ = (total_volume_ - volume) * (1 - (mask & 1));

        if (mask) {
            total_volume_ = 0;
            orders.clear();
            return;
        }

        int n = orders.size();
        int new_start = 0;

        for (int i = 0; i < n; ++i) {
            uint32_t order_vol = orders[i].second;

            uint32_t cond_mask = -static_cast<int32_t>(volume < order_vol);

            uint32_t new_order_vol = (order_vol - volume) & cond_mask;

            uint32_t new_volume = (volume & cond_mask) | ((volume - order_vol) & ~cond_mask);

            orders[i].second = new_order_vol;

            new_start += (1 - (cond_mask & 1));

            volume = new_volume;

            if (cond_mask) break;
        }

        orders.erase(orders.begin(), orders.begin() + new_start);
        total_volume_ -= volume;
    };

    inline uint32_t get_total_volume() const { return total_volume_; }

private:
    uint32_t total_volume_;
    std::vector<std::pair<uint32_t, uint32_t>> orders;
};

#endif
