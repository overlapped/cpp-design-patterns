#include "read_write_lock.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

int main() {
    arch::ThreadSafeCache<std::string, int> config_cache;
    config_cache.set("max_connections", 100);
    config_cache.set("timeout_ms", 500);

    // Запускаем 4 потока-читателя, которые постоянно читают конфигурацию
    std::vector<std::thread> readers;
    for (int id = 1; id <= 4; ++id) {
        readers.emplace_back([&config_cache, id]() {
            for (int iter = 0; iter < 3; ++iter) {
                auto val = config_cache.get("max_connections");
                std::cout << "[Reader " << id << "] max_connections = "
                          << val.value_or(-1) << '\n';
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        });
    }

    // Запускаем 1 поток-писатель, который редко обновляет конфигурацию
    std::thread writer([&config_cache]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        std::cout << ">>> [Writer] Updating max_connections to 250 <<<\n";
        config_cache.set("max_connections", 250);
    });

    for (auto& r : readers) {
        r.join();
    }
    writer.join();

    return 0;
}
