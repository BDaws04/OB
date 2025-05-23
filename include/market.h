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
              buy_book(1001, true, peg_price),
              sell_book(1001, false, peg_price),
              order_count(0),
              user_count(0) {};

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
        };
        void place_ioc_order(Order order);
        void place_gtc_order(Order order);

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