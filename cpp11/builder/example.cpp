#include "builder.hpp"
#include <cassert>
#include <iostream>

int main() {
    MargheritaBuilder m;
    Waiter w(&m);
    auto p = w.construct();
    std::cout << p.describe() << '\n';
    assert(p.topping == "mozzarella");
    return 0;
}