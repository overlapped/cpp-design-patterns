#include <gtest/gtest.h>
#include "builder.hpp"

TEST(BuilderTest, FluentBuild) {
    auto p = PizzaBuilder{}
        .dough("thick")
        .sauce("bbq")
        .topping("chicken")
        .build();
    EXPECT_EQ(p.dough, "thick");
    EXPECT_EQ(p.sauce, "bbq");
    EXPECT_EQ(p.topping, "chicken");
}

TEST(BuilderTest, PartialBuildUsesDefaults) {
    auto p = PizzaBuilder{}.dough("thin").build();
    EXPECT_EQ(p.dough, "thin");
    EXPECT_TRUE(p.sauce.empty());
}