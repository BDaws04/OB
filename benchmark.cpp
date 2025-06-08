#include "include/market.h"
#include "include/participant.h"

#include <chrono>
#include <thread>
#include <random> 
#include <memory>

int DEFAULT_PEG_PRICE = 1000;
int DEFAULT_LEVELS = 251;
int DEFAULT_PARTICIPANT_COUNT = 10;
int DEFAULT_ORDERS_PER_PARTICIPANT = 100000;

void simulate(const int peg_price, const int levels, const int participant_count, const int orders_per_participant) {
    std::vector<std::unique_ptr<Participant>> participants;

    auto market = std::make_shared<Market>(peg_price, levels);

    for (int i = 0; i < participant_count; ++i) {
        participants.emplace_back(std::make_unique<Participant>(i, market));
    }   

    int min_price = market->get_lower_bound(), max_price = market->get_upper_bound();

    std::thread market_simulator([market]() {
        market->simulate();
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::vector<std::thread> threads;
    auto start_time = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < participants.size(); ++i) {
        threads.emplace_back([&, i]() {
            thread_local std::mt19937 rng(12345 + i);

            std::uniform_int_distribution<uint32_t> price_dist(min_price, max_price);
            std::uniform_int_distribution<uint32_t> volume_dist(1, 100);
            std::uniform_int_distribution<int> buy_dist(0, 1);

            // Weighted distribution: [GTC, IOC, FOK, Market]
            // GTC: 70%, IOC: 15%, FOK: 5%, Market: 10%
            std::discrete_distribution<int> order_type_dist({70, 15, 5, 10});

            for (int j = 0; j < orders_per_participant; ++j) {
                uint32_t price = price_dist(rng);
                uint32_t volume = volume_dist(rng);
                bool is_buy = buy_dist(rng) == 1;
                int order_type = order_type_dist(rng);

                bool is_GTC = (order_type == 0);
                bool is_IOC = (order_type == 1);
                bool is_FOK = (order_type == 2);
                bool is_market = (order_type == 3);

                Order order(j, price, volume, is_buy, is_GTC, is_IOC, is_FOK, is_market);
                participants[i]->place_order(order);
            }
        });
    }


    for (auto& t : threads) {
        t.join();
    }

    market->stop();
    market_simulator.join();

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Total orders placed: " << market->get_order_count() << std::endl;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    std::cout << "------ Market Statistics ------" << std::endl;
    std::cout << "Successful Orders: " << market->get_success_orders() << std::endl;
    std::cout << "Failed Orders: " << market->get_failed_orders() << std::endl;

}


int main()
{
    simulate(DEFAULT_PEG_PRICE, 
             DEFAULT_LEVELS, 
             DEFAULT_PARTICIPANT_COUNT, 
             DEFAULT_ORDERS_PER_PARTICIPANT);
    return 0;
}