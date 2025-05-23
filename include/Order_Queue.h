#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include <queue>
#include <cstdint>
#include <atomic>
#include <mutex>
#include <optional>

template <typename T>
class OrderQueue {
    private:
        std::queue<T> queue_;
        SpinLock lock_;

    public:
        OrderQueue() = default;

        void push(const T& order) {
            lock_.lock();
            queue_.push(order);
            lock_.unlock();
        }

        T pop() {
            lock_.lock();
            if (!queue_.empty()) {
                T order = queue_.front();
                queue_.pop();
                lock_.unlock();
                return order;
            }
            lock_.unlock();
        }

        bool empty() {
            lock_.lock();
            bool result = queue_.empty();
            lock_.unlock();
            return result;
        }

        size_t size() {
            lock_.lock();
            size_t result = queue_.size();
            lock_.unlock();
            return result;
        }
};

class SpinLock {
public:
    SpinLock() : flag(ATOMIC_FLAG_INIT) {}

    void lock() {
        while (flag.test_and_set(std::memory_order_acquire)) {
        }
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }

    bool try_lock() {
        return !flag.test_and_set(std::memory_order_acquire);
    }

private:
    std::atomic_flag flag;
};


#endif