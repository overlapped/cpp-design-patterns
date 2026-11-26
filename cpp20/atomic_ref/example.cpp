#include "atomic_ref.hpp"
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    // 1. Stats: обычное поле, атомарный инкремент из многих потоков
    Stats s;
    std::vector<std::thread> threads;
    for (int i = 0; i < 8; ++i) {
        threads.emplace_back([&] {
            for (int j = 0; j < 1000; ++j) recordHit(s);
        });
    }
    for (auto& t : threads) t.join();
    assert(s.hits == 8000);

    // 2. Worker: обычный bool, атомарная запись/чтение
    Worker w;
    std::thread t([&] { runWorker(w); });
    while (!isDone(w)) {
        std::this_thread::yield();
    }
    t.join();
    assert(w.result == 42);

    // 3. Counter: CAS через atomic_ref
    Counter c;
    for (int i = 0; i < 5; ++i) {
        assert(tryIncrementIfLess(c, 3));
    }
    assert(!tryIncrementIfLess(c, 3));   // лимит достигнут
    assert(c.value == 3);

    std::cout << "atomic_ref: atomic view on non-atomic data\n";
    return 0;
}