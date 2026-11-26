#include "double_checked_locking.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

struct HeavyConfig {
    std::string endpoint;
    int retries;

    HeavyConfig(std::string ep, int r) : endpoint(std::move(ep)), retries(r) {
        std::cout << "[HeavyConfig] Expensive initialization running on thread "
                  << std::this_thread::get_id() << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
};

int main() {
    arch::LazyDclp<HeavyConfig> lazy_config([]() {
        return std::make_unique<HeavyConfig>("https://example.com", 3);
    });

    std::cout << "Initialized before access: " << std::boolalpha
              << lazy_config.is_initialized() << '\n';

    // Запускаем 4 потока, которые одновременно запрашивают конфигурацию.
    // Конструктор HeavyConfig выполнится ровно один раз.
    std::vector<std::thread> workers;
    for (int i = 0; i < 4; ++i) {
        workers.emplace_back([&lazy_config, i]() {
            const auto& cfg = lazy_config.get();
            std::cout << "Worker " << i << " got endpoint: " << cfg.endpoint
                      << " (retries=" << cfg.retries << ")\n";
        });
    }

    for (auto& w : workers) {
        w.join();
    }

    std::cout << "Initialized after access: " << lazy_config.is_initialized() << '\n';
    return 0;
}
