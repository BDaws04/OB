#ifndef ORDER_H
#define ORDER_H
#include <cstdint>

struct Order {
public:
    Order(uint32_t id,uint32_t price, uint32_t volume,
                 bool is_buy, bool is_GTC, bool is_IOC, bool is_FOK)
        : id_(id), price_(price), volume_(volume) {
        encode_metadata(is_buy, is_GTC, is_IOC, is_FOK);
    }

    uint32_t price() const { return price_; }
    uint32_t volume() const { return volume_; }
    MetaDataFlags metadata() const { return metadata_; }

private:
    uint32_t id_;
    uint32_t price_;
    uint32_t volume_;
    MetaDataFlags metadata_;

    inline void encode_metadata(bool is_buy, bool is_GTC, bool is_IOC, bool is_FOK) {
        metadata_ = MetaDataFlags::NONE;
        if (is_buy) metadata_ = metadata_ | MetaDataFlags::IS_BUY;
        if (is_GTC) metadata_ = metadata_ | MetaDataFlags::IS_GTC;
        if (is_IOC) metadata_ = metadata_ | MetaDataFlags::IS_IOC;
        if (is_FOK) metadata_ = metadata_ | MetaDataFlags::IS_FOK;
    }
};

enum class MetaDataFlags : uint32_t {
    NONE = 0,
    IS_BUY = 1 << 0,
    IS_GTC = 1 << 1,
    IS_IOC = 1 << 2,
    IS_FOK = 1 << 3
};

inline MetaDataFlags operator|(MetaDataFlags lhs, MetaDataFlags rhs) {
    return static_cast<MetaDataFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

inline MetaDataFlags& operator|=(MetaDataFlags& lhs, MetaDataFlags rhs) {
    lhs = lhs | rhs;
    return lhs;
}


#endif