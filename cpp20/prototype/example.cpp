#include "prototype.hpp"
#include <cassert>

int main() {
    Square s(4);
    auto copy = s.clone();
    auto* ss = dynamic_cast<Square*>(copy.get());
    assert(ss && ss->side == 4);
    return 0;
}