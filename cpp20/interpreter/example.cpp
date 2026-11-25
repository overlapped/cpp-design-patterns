#include "interpreter.hpp"
#include <cassert>
#include <iostream>

int main() {
    // (x + 3) * (y - 1), где x=2, y=5 → 5 * 4 = 20
    auto expr = binary(Op::Mul,
        binary(Op::Add, var("x"), num(3)),
        binary(Op::Sub, var("y"), num(1)));

    Context ctx;
    ctx.set("x", 2);
    ctx.set("y", 5);
    int result = expr->interpret(ctx);
    std::cout << "(x + 3) * (y - 1) = " << result << '\n';
    assert(result == 20);
    return 0;
}