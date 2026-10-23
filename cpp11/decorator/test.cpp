#include <gtest/gtest.h>
#include "decorator.hpp"

TEST(DecoratorTest, StacksDecorators) {
    std::unique_ptr<Coffee> c(new Sugar(std::unique_ptr<Coffee>(new Milk(std::unique_ptr<Coffee>(new Espresso())))));
    EXPECT_EQ(c->cost(), 2 + 1 + 1);
    EXPECT_EQ(c->description(), "Espresso + Milk + Sugar");
}