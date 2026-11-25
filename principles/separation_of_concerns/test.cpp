#include <gtest/gtest.h>
#include "separation_of_concerns.hpp"
#include <stdexcept>

TEST(SoCTest, ParseAlone) {
    auto v = good::parseCsv("1,2,3");
    EXPECT_EQ(v, (std::vector<int>{1, 2, 3}));
}

TEST(SoCTest, ValidateAlone) {
    EXPECT_NO_THROW(good::validateNonNegative({1, 2, 3}));
    EXPECT_THROW(good::validateNonNegative({1, -2, 3}), std::runtime_error);
}

TEST(SoCTest, SumAlone) {
    EXPECT_EQ(good::sum({1, 2, 3}), 6);
    EXPECT_EQ(good::sum({}), 0);
}

TEST(SoCTest, FormatAlone) {
    EXPECT_EQ(good::formatSum(42), "sum=42");
}

TEST(SoCTest, ComposedMatchesBadVersion) {
    EXPECT_EQ(good::processCsvLine("1,2,3"), "sum=6");
    EXPECT_THROW(good::processCsvLine("1,-2"), std::runtime_error);
}