#include "srp.hpp"
#include <cassert>
#include <iostream>

int main() {
    bad::User  bu("Alice", 30);
    good::User gu{"Alice", 30};
    assert(bu.toJson() == good::toJson(gu));
    std::cout << "SRP: split classes produce same JSON\n";
    return 0;
}