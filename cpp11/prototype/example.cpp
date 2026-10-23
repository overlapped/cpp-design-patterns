#include "prototype.hpp"
#include <cassert>

int main() {
    Circle c(5);
    auto copy = c.clone();
    auto* cc = dynamic_cast<Circle*>(copy.get());
    assert(cc && cc->radius == 5);
    return 0;
}