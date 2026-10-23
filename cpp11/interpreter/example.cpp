#include "interpreter.hpp"
#include <cassert>
#include <iostream>

int main() {
    // (2 + 3) * 4 = 20
    std::unique_ptr<Expression> expr(new MultiplyExpression(
        std::unique_ptr<Expression>(new AddExpression(num(2), num(3))),
        num(4)));

    Context ctx;
    int result = expr->interpret(ctx);
    std::cout << "(2 + 3) * 4 = " << result << '\n';
    assert(result == 20);
    return 0;
}