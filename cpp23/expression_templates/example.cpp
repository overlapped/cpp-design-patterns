#include "expression_templates.hpp"
#include <cassert>
#include <iostream>

int main() {
    Vec a = makeVec({1, 2, 3});
    Vec b = makeVec({10, 20, 30});
    Vec c = makeVec({100, 200, 300});
    Vec result(3);

    // Без ET: tmp1 = a + b; tmp2 = tmp1 + c; result = tmp2;
    // С ET: result[i] = a[i] + b[i] + c[i] — один проход
    assign(result, a + b + c);

    assert(result[0] == 111);
    assert(result[1] == 222);
    assert(result[2] == 333);

    // Смешанное выражение: a + b * c
    assign(result, a + b * c);
    assert(result[0] == 1 + 10 * 100);
    assert(result[1] == 2 + 20 * 200);
    assert(result[2] == 3 + 30 * 300);

    // += с выражением
    Vec acc = makeVec({1, 1, 1});
    acc += a + b;
    assert(acc[0] == 1 + 1 + 10);
    assert(acc[1] == 1 + 2 + 20);
    assert(acc[2] == 1 + 3 + 30);

    std::cout << "Expression Templates: lazy evaluation without temporaries\n";
    return 0;
}