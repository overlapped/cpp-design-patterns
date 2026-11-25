#include <gtest/gtest.h>
#include "prototype.hpp"

TEST(PrototypeTest, ClonesIndependently) {
    Circle c(7);
    auto copy = c.clone();
    auto* cc = dynamic_cast<Circle*>(copy.get());
    ASSERT_NE(cc, nullptr);
    EXPECT_EQ(cc->radius, 7);
    cc->radius = 9;
    EXPECT_EQ(c.radius, 7);
}

TEST(PrototypeTest, Square) {
    Square s(3);
    auto copy = s.clone();
    EXPECT_EQ(copy->name(), "Square");
}