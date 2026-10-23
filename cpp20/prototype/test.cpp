#include <gtest/gtest.h>
#include "prototype.hpp"

TEST(PrototypeTest, CloneKeepsData) {
    Circle c(11);
    auto copy = c.clone();
    auto* cc = dynamic_cast<Circle*>(copy.get());
    ASSERT_NE(cc, nullptr);
    EXPECT_EQ(cc->radius, 11);
}

TEST(PrototypeTest, CloneIsIndependent) {
    Circle c(3);
    auto copy = c.clone();
    auto* cc = dynamic_cast<Circle*>(copy.get());
    cc->radius = 42;
    EXPECT_EQ(c.radius, 3);
}