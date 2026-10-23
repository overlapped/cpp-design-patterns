#include "proxy.hpp"
#include <cassert>

int main() {
    LazyImageProxy p("cat.png");
    assert(!p.loaded());
    assert(p.display() == "real:cat.png");
    assert(p.loaded());
    return 0;
}