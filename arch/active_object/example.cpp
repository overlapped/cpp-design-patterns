#include "active_object.hpp"
#include <cassert>
#include <iostream>

int main() {
    ActiveCounter counter;

    // Асинхронные вызовы — каждый возвращает future
    auto f1 = counter.increment();
    auto f2 = counter.increment();
    auto f3 = counter.increment();

    // Дожидаемся всех
    f1.get();
    f2.get();
    f3.get();

    // Синхронный get
    auto g = counter.get();
    assert(g.get() == 3);

    std::cout << "Active Object example OK\n";
    return 0;
}