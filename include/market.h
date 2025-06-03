#ifndef MARKET_H
#define MARKET_H


#include "book.h"
#include "order.h"
#include "order_queue.h"
#include "participant.h"


struct Market {
    public:
        Market(uint32_t peg_price)
            : peg_price(peg_price),
              buy_book(1001, peg_price),
              sell_book(1001, peg_price),
              order_count(0),
              user_count(0) {};

        inline int get_index(uint32_t price) const {
            if (price < buy_book.lower_bound || price > buy_book.upper_bound) {
                return -1; 
            }
            return price - buy_book.lower_bound;
        };

        void add_order(Order order){
            orders.push(order);
        };

        void simulate(){
            Order current = orders.pop();
            current.id = order_count++;

            if (current.is_GTC()){
                place_gtc_order(current);
            }
            else if (current.is_IOC()){
                place_ioc_order(current);
            }
            else if (current.is_FOK()){
                place_fok_order(current);
            }

        };

        void place_fok_order(Order order){
            int index = get_index(order.price());
            if (index < 0) {
                return;
            }
            if (order.is_buy()) {
                if (sell_book.price_levels[index].get_total_volume() <= order.volume()) {
                    sell_book.price_levels[index].remove_volume(order.volume());
                    buy_book.price_levels[index].add_order(order.id, order.volume());
                } else {

                    return;
                }

            } else {
                if (buy_book.price_levels[index].get_total_volume() <= order.volume()) {
                    buy_book.price_levels[index].remove_volume(order.volume());
                    sell_book.price_levels[index].add_order(order.id, order.volume());
                } else {
                    return;
                }
            }
        };
        void place_ioc_order(Order order){
            int index = get_index(order.price());
            if (index < 0) {
                return;
            }
            if (order.is_buy()) {
                if (sell_book.price_levels[index].get_total_volume() > 0) {
                    int removed_volume = std::min(order.volume(), sell_book.price_levels[index].get_total_volume());
                    sell_book.price_levels[index].remove_volume(removed_volume);
                    if (removed_volume != order.volume()) {
                        buy_book.price_levels[index].add_order(order.id, order.volume() - removed_volume);
                    }
                } else {
                    return;
                }
            } else {
                if (buy_book.price_levels[index].get_total_volume() > 0) {
                    int removed_volume = std::min(order.volume(), buy_book.price_levels[index].get_total_volume());
                    buy_book.price_levels[index].remove_volume(removed_volume);
                    if (removed_volume != order.volume()) {
                        sell_book.price_levels[index].add_order(order.id, order.volume() - removed_volume);
                    }
                } else {
                    return;
                }
            }
        };
        void place_gtc_order(Order order){
            int index = get_index(order.price());
            if (index < 0) {
                return;
            }
            if (order.is_buy()) {
                buy_book.price_levels[index].add_order(order.id, order.volume());
            } else {
                sell_book.price_levels[index].add_order(order.id, order.volume());
            }
        };

        Participant* create_participant(uint32_t user_id){
            Participant* participant = new Participant(user_id, * this);
            return participant;
        }
        void remove_participant(Participant* participant){
            delete participant;
        }
    private:
        OrderQueue<Order> orders;
        Book buy_book;
        Book sell_book; 

        uint64_t order_count;   
        uint64_t user_count;
        uint32_t peg_price;
};



#endif