#include "law_of_demeter.hpp"
#include <cassert>
#include <iostream>

int main() {
    bad::Person bp;
    good::Person gp;

    assert(bad::getAmount(bp) == good::getAmount(gp));
    std::cout << "Law of Demeter: same amount, fewer dependencies\n";
    return 0;
}