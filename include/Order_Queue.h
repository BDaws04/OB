#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include <queue>
#include <cstdint>
#include <atomic>
#include <mutex>
#include <optional>
#include <condition_variable>



template<typename T>
class OrderQueue {
private:
    std::queue<T> queue_;
    std::mutex mutex_;                   
    std::condition_variable cond_;         

public:
    void push(const T& item) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            queue_.push(item);
        }
        cond_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [&]() { return !queue_.empty(); });
        T item = queue_.front();
        queue_.pop();
        return item;
    }
};


#endif