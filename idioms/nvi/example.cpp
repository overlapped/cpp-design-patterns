#include "nvi.hpp"
#include <cassert>
#include <iostream>

int main() {
    Circle c(2.0);
    Rectangle r(3.0, 4.0);

    std::cout << c.describe() << '\n';
    std::cout << r.describe() << '\n';

    assert(c.scaledArea(2.0) > 12.5);   // 4π ≈ 12.566
    assert(r.scaledArea(0.5) == 6.0);   // 12 * 0.5
    assert(r.scaledArea(-1.0) == 0.0);  // защита в базовом методе

    std::cout << "NVI example OK\n";
    return 0;
}