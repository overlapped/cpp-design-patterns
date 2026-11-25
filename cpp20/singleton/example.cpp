#include "singleton.hpp"
#include <cassert>
#include <thread>
#include <vector>

int main() {
    std::vector<Singleton*> ptrs(8);
    std::vector<std::thread> threads;
    for (int i = 0; i < 8; ++i)
        threads.emplace_back([&, i] { ptrs[i] = &Singleton::instance(); });
    for (auto& t : threads) t.join();
    for (auto* p : ptrs) assert(p == ptrs[0]);
    return 0;
}