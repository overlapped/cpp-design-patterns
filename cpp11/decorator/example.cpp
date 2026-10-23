#include "decorator.hpp"
#include <cassert>

int main() {
    std::unique_ptr<Coffee> c(new Sugar(std::unique_ptr<Coffee>(new Milk(std::unique_ptr<Coffee>(new Espresso())))));
    assert(c->cost() == 4);
    return 0;
}