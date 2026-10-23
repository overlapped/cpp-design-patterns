#include <gtest/gtest.h>
#include "builder.hpp"

TEST(BuilderTest, Margherita) {
    MargheritaBuilder b;
    Waiter w(&b);
    auto p = w.construct();
    EXPECT_EQ(p.dough, "thin");
    EXPECT_EQ(p.topping, "mozzarella");
}

TEST(BuilderTest, Pepperoni) {
    PepperoniBuilder b;
    Waiter w(&b);
    auto p = w.construct();
    EXPECT_EQ(p.dough, "thick");
    EXPECT_EQ(p.topping, "pepperoni");
}