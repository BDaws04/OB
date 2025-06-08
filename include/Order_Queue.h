#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include <atomic>
#include <memory>
#include <optional>

template<typename T>
struct Node {
    std::optional<T> data;
    std::atomic<Node*> next;

    Node() : next(nullptr) {}
    Node(T val) : data(std::move(val)), next(nullptr) {}
};

template<typename T>
class OrderQueue {
private:
    std::atomic<Node<T>*> head; 
    Node<T>* tail;               
public:
    OrderQueue() {
        Node<T>* dummy = new Node<T>(); 
        head.store(dummy, std::memory_order_relaxed);
        tail = dummy;
    }

    ~OrderQueue() {
        while (pop()) {} 
        delete tail;
    }

    void push(const T& value) {
        Node<T>* new_node = new Node<T>(value);
        Node<T>* prev_head = head.exchange(new_node, std::memory_order_acq_rel);
        prev_head->next.store(new_node, std::memory_order_release);
    }

    std::optional<T> pop() {
        Node<T>* next = tail->next.load(std::memory_order_acquire);
        if (next) {
            std::optional<T> result = std::move(next->data);
            delete tail;
            tail = next;
            return result;
        }
        return std::nullopt;
    }

    bool empty() const {
        return tail->next.load(std::memory_order_acquire) == nullptr;
    }
};

#endif
