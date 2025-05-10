#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include <atomic>

template <typename T>
class OrderQueue {
    public:
        ...
    private:
        // power of 2 buffer size for fast modulo operation
        // this is a circular buffer, so the size must be a power of 2
        const size_t _bufer_size = 1024; 
};


#endif