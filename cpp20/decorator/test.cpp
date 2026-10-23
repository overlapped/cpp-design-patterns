#include <gtest/gtest.h>
#include "decorator.hpp"

TEST(DecoratorTest, StacksDecorators) {
    auto c = std::make_unique<Sugar>(
        std::make_unique<Milk>(
            std::make_unique<Espresso>()));
    EXPECT_EQ(c->cost(), 4);
    EXPECT_EQ(c->description(), "Espresso + Milk + Sugar");
}