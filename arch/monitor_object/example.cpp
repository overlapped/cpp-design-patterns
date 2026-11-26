#include "monitor_object.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    // Очередь максимум на 3 элемента
    arch::BoundedMonitorQueue<int> queue(3);

    // Поток-производитель (Producer)
    std::thread producer([&queue]() {
        for (int i = 1; i <= 6; ++i) {
            std::cout << "[Producer] Pushing: " << i << '\n';
            queue.push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
        std::cout << "[Producer] Closing queue.\n";
        queue.close();
    });

    // Поток-потребитель (Consumer) работает медленнее производителя
    std::thread consumer([&queue]() {
        while (auto item = queue.pop()) {
            std::cout << "  [Consumer] Popped: " << *item << '\n';
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }
        std::cout << "  [Consumer] Queue closed and empty. Exiting.\n";
    });

    producer.join();
    consumer.join();

    return 0;
}
