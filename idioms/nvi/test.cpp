#include <gtest/gtest.h>
#include "nvi.hpp"
#include <string>

TEST(NviTest, DescribeUsesVirtuals) {
    Circle c(1.0);
    std::string d = c.describe();
    EXPECT_NE(d.find("Circle"), std::string::npos);
    EXPECT_NE(d.find("area="), std::string::npos);
}

TEST(NviTest, ScaledAreaPositive) {
    Rectangle r(2.0, 3.0);
    EXPECT_DOUBLE_EQ(r.scaledArea(2.0), 12.0);
}

TEST(NviTest, ScaledAreaNegativeFactorReturnsZero) {
    Rectangle r(2.0, 3.0);
    EXPECT_DOUBLE_EQ(r.scaledArea(-5.0), 0.0);
}

TEST(NviTest, PolymorphicThroughBase) {
    const Shape& s = Circle(1.0);
    EXPECT_NE(s.describe().find("Circle"), std::string::npos);
}