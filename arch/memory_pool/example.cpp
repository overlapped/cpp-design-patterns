#include "memory_pool.hpp"
#include <cassert>
#include <iostream>

struct Particle {
    int id;
    double x, y;
    Particle(int i, double xx, double yy) : id(i), x(xx), y(yy) {}
};

int main() {
    MemoryPool<Particle, 4> pool;
    assert(pool.available() == 4);

    Particle* a = pool.create(1, 0.0, 0.0);
    Particle* b = pool.create(2, 1.0, 1.0);
    assert(a && b);
    assert(pool.available() == 2);

    assert(a->id == 1 && b->x == 1.0);

    pool.destroy(a);
    assert(pool.available() == 3);

    // Блок переиспользуется
    Particle* c = pool.create(3, 2.0, 2.0);
    assert(c == a);   // тот же адрес — free-list вернул блок

    // Исчерпание пула
    pool.destroy(b);
    pool.destroy(c);
    pool.create(0, 0, 0);
    pool.create(0, 0, 0);
    pool.create(0, 0, 0);
    pool.create(0, 0, 0);
    assert(pool.available() == 0);
    assert(pool.create(0, 0, 0) == nullptr);

    std::cout << "Memory Pool example OK\n";
    return 0;
}