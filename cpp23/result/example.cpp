#include "result.hpp"
#include <cassert>
#include <iostream>

int main() {
    auto ok = process_age("25");
    assert(ok.has_value());
    std::cout << ok.value() << '\n';

    auto bad = process_age("abc");
    assert(!bad.has_value());
    std::cout << bad.error() << '\n';

    auto range = process_age("999");
    assert(!range.has_value());
    std::cout << range.error() << '\n';

    return 0;
}