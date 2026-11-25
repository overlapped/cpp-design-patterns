#include "builder.hpp"
#include <cassert>

int main() {
    auto p = PizzaBuilder{}
        .dough("thin")
        .sauce("tomato")
        .topping("mozzarella")
        .build();
    assert(p.topping == "mozzarella");
    return 0;
}