#include "include/market.h"
#include <chrono>
#include <iostream>
#include "include/participant.h"
#include "thread"


int main()
{
    std::vector<Participant*> participants;
    Market market(1000); // Create a market with a peg price of 1000

    for (int i = 0; i < 10; i++)
    {
        participants.emplace_back(new Participant(i, market));
    }
    
    int min_price = market.get_lower_bound();
    int max_price = market.get_upper_bound();

    std::thread market_simulator(&Market::simulate, &market);

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    for (int i = 0; i < 100000; i++){
        for (int j = 0; j < participants.size(); j++)
        {
            uint32_t price = min_price + (rand() % (max_price - min_price + 1));
            uint32_t volume = rand() % 100 + 1; // Random volume between 1 and 100
            bool is_buy = rand() % 2; // Randomly choose buy or sell
            bool is_GTC = rand() % 2; // Randomly choose GTC
            bool is_IOC = rand() % 2; // Randomly choose IOC
            bool is_FOK = rand() % 2; // Randomly choose FOK

            Order order(i, price, volume, is_buy, is_GTC, is_IOC, is_FOK);
            participants[j]->place_order(order);
        }
    }

    market_simulator.join();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Time taken for 1000000 orders: " << elapsed_seconds.count() << " seconds\n";
    std::cout << "Total orders placed: " << market.get_order_count() << "\n";
    for (auto& participant : participants) {
        delete participant; // Clean up dynamically allocated participants
    }
}
