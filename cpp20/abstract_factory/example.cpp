#include "abstract_factory.hpp"
#include <cassert>

int main() {
    MacFactory f;
    auto b = f.createButton();
    auto c = f.createCheckbox();
    assert(b && c);
    return 0;
}