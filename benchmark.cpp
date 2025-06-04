#include "include/market.h"
#include <chrono>
#include <iostream>
#include "include/participant.h"
#include <thread>
#include <random> 
#include <windows.h>

int main()
{
    std::vector<Participant*> participants;
    Market market(1000);

    for (int i = 0; i < 10; i++)
    {
        participants.emplace_back(new Participant(i, market));
    }
    
    int min_price = market.get_lower_bound();
    int max_price = market.get_upper_bound();

    std::thread market_simulator(&Market::simulate, &market);

    market_simulator.detach();

    const int orders_per_participant = 100000;
    std::vector<std::thread> threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < participants.size(); ++i) {
        threads.emplace_back([&, i]() {
            thread_local std::mt19937 rng(std::random_device{}() + i);

            std::uniform_int_distribution<uint32_t> price_dist(min_price, max_price);
            std::uniform_int_distribution<uint32_t> volume_dist(1, 100);
            std::uniform_int_distribution<int> buy_dist(0, 1);
            std::uniform_int_distribution<int> order_type_dist(0, 2);

            for (int j = 0; j < orders_per_participant; ++j) {
                uint32_t price = price_dist(rng);
                uint32_t volume = volume_dist(rng);
                bool is_buy = buy_dist(rng) == 1;
                int order_type = order_type_dist(rng);

                bool is_GTC = (order_type == 0);
                bool is_IOC = (order_type == 1);
                bool is_FOK = (order_type == 2);

                Order order(j, price, volume, is_buy, is_GTC, is_IOC, is_FOK);
                participants[i]->place_order(order);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Total orders placed: " << market.get_order_count() << std::endl;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    std::cout << "------ Market Statistics ----" << std::endl;
    std::cout << "Successful Orders: " << market.get_success_orders() << std::endl;
    std::cout << "Failed Orders: " << market.get_failed_orders() << std::endl;

}

