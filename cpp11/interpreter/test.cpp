#include <gtest/gtest.h>
#include "interpreter.hpp"

TEST(InterpreterTest, Literal) {
    Context ctx;
    EXPECT_EQ(num(42)->interpret(ctx), 42);
}

TEST(InterpreterTest, Addition) {
    Context ctx;
    AddExpression e(num(2), num(3));
    EXPECT_EQ(e.interpret(ctx), 5);
}

TEST(InterpreterTest, PrecedenceByTree) {
    // (2 + 3) * 4
    Context ctx;
    MultiplyExpression e(
        std::unique_ptr<Expression>(new AddExpression(num(2), num(3))),
        num(4));
    EXPECT_EQ(e.interpret(ctx), 20);
}

TEST(InterpreterTest, Variables) {
    Context ctx;
    ctx.set("x", 10);
    ctx.set("y", 4);
    SubtractExpression e(
        std::unique_ptr<Expression>(new VariableExpression("x")),
        std::unique_ptr<Expression>(new VariableExpression("y")));
    EXPECT_EQ(e.interpret(ctx), 6);
}

TEST(InterpreterTest, UnknownVariableThrows) {
    Context ctx;
    VariableExpression e("missing");
    EXPECT_THROW(e.interpret(ctx), std::runtime_error);
}

TEST(InterpreterTest, DivisionByZeroThrows) {
    Context ctx;
    DivideExpression e(num(1), num(0));
    EXPECT_THROW(e.interpret(ctx), std::runtime_error);
}