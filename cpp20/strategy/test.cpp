#include <gtest/gtest.h>
#include "strategy.hpp"

TEST(StrategyTest, FunctionBased) {
    std::vector<int> v{3, 1, 2};
    Sorter s;
    s.setStrategy(ascendingSort);
    s.sort(v);
    EXPECT_EQ(v, (std::vector<int>{1, 2, 3}));
    s.setStrategy(descendingSort);
    s.sort(v);
    EXPECT_EQ(v, (std::vector<int>{3, 2, 1}));
}