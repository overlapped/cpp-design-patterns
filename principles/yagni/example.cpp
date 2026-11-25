#include "yagni.hpp"
#include <cassert>
#include <iostream>

int main() {
    bad::Report  b;
    good::Report g;
    b.addLine("hello"); g.addLine("hello");
    b.addLine("world"); g.addLine("world");
    assert(b.lineCount() == g.lineCount());
    std::cout << "YAGNI: minimal class does the same job\n";
    return 0;
}