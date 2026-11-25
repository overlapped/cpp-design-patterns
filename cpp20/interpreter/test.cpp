#include <gtest/gtest.h>
#include "interpreter.hpp"

TEST(InterpreterTest, Literal) {
    Context ctx;
    EXPECT_EQ(num(42)->interpret(ctx), 42);
}

TEST(InterpreterTest, Addition) {
    Context ctx;
    auto e = binary(Op::Add, num(2), num(3));
    EXPECT_EQ(e->interpret(ctx), 5);
}

TEST(InterpreterTest, PrecedenceByTree) {
    Context ctx;
    auto e = binary(Op::Mul,
        binary(Op::Add, num(2), num(3)),
        num(4));
    EXPECT_EQ(e->interpret(ctx), 20);
}

TEST(InterpreterTest, Variables) {
    Context ctx;
    ctx.set("x", 10);
    ctx.set("y", 4);
    auto e = binary(Op::Sub, var("x"), var("y"));
    EXPECT_EQ(e->interpret(ctx), 6);
}

TEST(InterpreterTest, UnknownVariableThrows) {
    Context ctx;
    auto e = var("missing");
    EXPECT_THROW(e->interpret(ctx), std::runtime_error);
}

TEST(InterpreterTest, DivisionByZeroThrows) {
    Context ctx;
    auto e = binary(Op::Div, num(1), num(0));
    EXPECT_THROW(e->interpret(ctx), std::runtime_error);
}