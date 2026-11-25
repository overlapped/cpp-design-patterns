#include "crtp.hpp"
#include <cassert>
#include <iostream>

int main() {
    Name n{"Alice"};
    Point p{3, 4};

    // Оба вызывают базовый print/describe, но с разными self
    n.print();
    p.print();

    assert(n.describe() == "[Alice]");
    assert(p.describe() == "[(3, 4)]");

    std::cout << "deducing this example OK\n";
    return 0;
}