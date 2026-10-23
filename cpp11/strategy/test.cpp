#include <gtest/gtest.h>
#include "strategy.hpp"

TEST(StrategyTest, SwitchesAlgorithm) {
    std::vector<int> v{3, 1, 2};
    Sorter s;
    s.setStrategy(std::unique_ptr<SortStrategy>(new AscendingSort()));
    s.sort(v);
    EXPECT_EQ(v, (std::vector<int>{1, 2, 3}));
    s.setStrategy(std::unique_ptr<SortStrategy>(new DescendingSort()));
    s.sort(v);
    EXPECT_EQ(v, (std::vector<int>{3, 2, 1}));
}