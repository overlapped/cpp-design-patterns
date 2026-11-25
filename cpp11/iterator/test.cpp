#include <gtest/gtest.h>
#include "iterator.hpp"
#include <vector>

TEST(IteratorTest, IteratesRange) {
    std::vector<int> out;
    for (int v : IntRange(1, 4)) out.push_back(v);
    EXPECT_EQ(out, (std::vector<int>{1, 2, 3}));
}

TEST(IteratorTest, EmptyRange) {
    int count = 0;
    for (int v : IntRange(5, 5)) { (void)v; ++count; }
    EXPECT_EQ(count, 0);
}