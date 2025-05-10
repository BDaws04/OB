#include "../include/Order.h"

void Order::encode_metadata(bool is_buy, bool is_GTC, bool is_IOC, bool is_FOK) {
    metadata = 0;
    metadata |= (is_buy  ? (1 << 0) : 0);
    metadata |= (is_GTC  ? (1 << 1) : 0);
    metadata |= (is_IOC  ? (1 << 2) : 0);
    metadata |= (is_FOK  ? (1 << 3) : 0);
}

bool Order::is_buy() const {
    return (metadata & (1 << 0)) != 0;
}

bool Order::is_GTC() const {
    return (metadata & (1 << 1)) != 0;
}

bool Order::is_IOC() const {
    return (metadata & (1 << 2)) != 0;
}

bool Order::is_FOK() const {
    return (metadata & (1 << 3)) != 0;
}
