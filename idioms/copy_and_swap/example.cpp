#include "copy_and_swap.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

int main() {
    String a("hello");
    String b("world");

    b = a;
    assert(std::strcmp(b.c_str(), "hello") == 0);
    assert(std::strcmp(a.c_str(), "hello") == 0);

    // Self-assignment
    a = a;
    assert(std::strcmp(a.c_str(), "hello") == 0);

    // Move
    String c(std::move(a));
    assert(std::strcmp(c.c_str(), "hello") == 0);

    std::cout << "Copy-and-Swap example OK\n";
    return 0;
}