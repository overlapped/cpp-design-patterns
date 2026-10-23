#include <gtest/gtest.h>
#include "iterator.hpp"
#include <ranges>
#include <vector>

TEST(IteratorTest, IteratesRange) {
    std::vector<int> out;
    for (int v : IntRange(1, 4)) out.push_back(v);
    EXPECT_EQ(out, (std::vector<int>{1, 2, 3}));
}

TEST(IteratorTest, WorksWithRanges) {
    std::vector<int> out;
    for (int v : IntRange(1, 6) | std::views::filter([](int n) { return n % 2 == 0; }))
        out.push_back(v);
    EXPECT_EQ(out, (std::vector<int>{2, 4}));
}