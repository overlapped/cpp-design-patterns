#include "rule_of_five.hpp"
#include <cassert>
#include <utility>

int main() {
    Buffer a(3);
    a[0] = 1; a[1] = 2; a[2] = 3;

    // Копия — независимые данные
    Buffer b = a;
    b[0] = 100;
    assert(a[0] == 1);
    assert(b[0] == 100);

    // Перемещение — источник опустошается
    Buffer c = std::move(a);
    assert(a.size() == 0);
    assert(c.size() == 3);
    assert(c[0] == 1);

    // Copy-and-swap в присваивании
    Buffer d;
    d = b;
    assert(d.size() == 3 && d[0] == 100);

    return 0;
}