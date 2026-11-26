#include "thread_pool.hpp"

#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    arch::ThreadPool pool(4);
    std::cout << "ThreadPool started with " << pool.worker_count() << " workers.\n";

    std::vector<std::future<int>> futures;

    // Отправляем 8 вычислительных задач
    for (int i = 1; i <= 8; ++i) {
        futures.push_back(pool.submit([i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            return i * i;
        }));
    }

    // Собираем результаты по мере готовности
    int total_sum = 0;
    for (std::size_t i = 0; i < futures.size(); ++i) {
        int res = futures[i].get();
        std::cout << "Task " << (i + 1) << " result: " << res << '\n';
        total_sum += res;
    }

    std::cout << "Total sum of squares: " << total_sum << '\n';
    return 0;
}
