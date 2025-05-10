#ifndef ORDER_H
#define ORDER_H

#include <cstdint>

struct alignas(16) Order {
    public:
        bool is_buy() const;
        bool is_GTC() const;
        bool is_IOC() const;
        bool is_FOK() const;
        uint32_t id;
        uint32_t price;
        uint32_t volume;
        Order(uint32_t id, uint32_t price, uint32_t volume, bool is_buy, bool is_GTC, bool is_IOC, bool is_FOK)
            : id(id), price(price), volume(volume) {
                encode_metadata(is_buy, is_GTC, is_IOC, is_FOK); 
         }
    private:
        uint32_t metadata;
        void encode_metadata(bool is_buy, bool is_GTC, bool is_IOC, bool is_FOK);
};

#endif