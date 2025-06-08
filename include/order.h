#ifndef ORDER_H
#define ORDER_H

#include <cstdint>

struct alignas(16) Order {
public:
    uint32_t id;

    Order() : metadata_(0) {};

    Order(uint32_t id,uint32_t price, uint32_t volume,
                 bool is_buy, bool is_GTC, bool is_IOC, bool is_FOK, bool is_market)
        : id(id), price_(price), volume_(volume) {
        encode_metadata(is_buy, is_GTC, is_IOC, is_FOK, is_market);
    }

    inline uint32_t price() const { return price_; }
    inline uint32_t volume() const { return volume_; }
    inline uint32_t metadata() const { return metadata_; }
    inline bool is_buy() const { return metadata_ & (1 << 0); }
    inline bool is_GTC() const { return metadata_ & (1 << 1); }
    inline bool is_IOC() const { return metadata_ & (1 << 2); }
    inline bool is_FOK() const { return metadata_ & (1 << 3); }
    inline bool is_Market() const { return metadata_ & (1 << 4); }
 
private:
    uint32_t price_;
    uint32_t volume_;
    // Metadata is a bit field:
    // Bit 0: is_buy
    // Bit 1: is_GTC
    // Bit 2: is_IOC
    // Bit 3: is_FOK
    // Bit 4: is_market
    uint32_t metadata_;

    inline void encode_metadata(bool is_buy, bool is_GTC, bool is_IOC, bool is_FOK, bool is_market) {
        metadata_ = 0;
        if (is_buy) {
            metadata_ |= 1 << 0;
        }
        if (is_GTC) {
            metadata_ |= 1 << 1;
        }
        if (is_IOC) {
            metadata_ |= 1 << 2; 
        }
        if (is_FOK) {
            metadata_ |= 1 << 3;
        }
        if (is_market) {
            metadata_ |= 1 << 4;
        }
    }
};

#endif