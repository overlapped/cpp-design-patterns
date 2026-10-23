#include "decorator.hpp"
#include <cassert>

int main() {
    auto c = std::make_unique<Sugar>(
        std::make_unique<Milk>(
            std::make_unique<Espresso>()));
    assert(c->cost() == 4);
    return 0;
}