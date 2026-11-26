#include "policy_based_design.hpp"
#include <cassert>
#include <iostream>

int main() {
    // 1. ConsoleLogger + VectorStorage + NoCheck
    Repository<int> r1;
    r1.add(1);
    r1.add(2);
    assert(r1.size() == 2);

    // 2. SilentLogger + ArrayStorage + PositiveCheck
    Repository<int, SilentLogger, ArrayStorage, PositiveCheck<int>> r2;
    r2.add(10);    // принято
    r2.add(-5);    // отклонено (PositiveCheck)
    r2.add(20);    // принято
    assert(r2.size() == 2);
    assert(r2.get(0) == 10);
    assert(r2.get(1) == 20);

    // 3. Комбинация: ConsoleLogger + ArrayStorage + PositiveCheck
    Repository<int, ConsoleLogger, ArrayStorage, PositiveCheck<int>> r3;
    r3.add(42);
    r3.add(0);     // отклонено
    assert(r3.size() == 1);

    std::cout << "Policy-Based Design: behavior via policies\n";
    return 0;
}