#include <gtest/gtest.h>
#include "generator.hpp"
#include <ranges>
#include <stdexcept>

TEST(GeneratorTest, FibonacciFirstTen) {
    std::vector<long long> v;
    for (auto x : fibonacci() | std::views::take(10)) v.push_back(x);
    EXPECT_EQ(v, (std::vector<long long>{0,1,1,2,3,5,8,13,21,34}));
}

TEST(GeneratorTest, IotaRange) {
    std::vector<int> v;
    for (int x : iota(0, 5)) v.push_back(x);
    EXPECT_EQ(v, (std::vector<int>{0,1,2,3,4}));
}

TEST(GeneratorTest, IotaEmpty) {
    std::vector<int> v;
    for (int x : iota(5, 5)) v.push_back(x);
    EXPECT_TRUE(v.empty());
}

TEST(GeneratorTest, PrimesFirstTen) {
    std::vector<int> v;
    for (int x : primes() | std::views::take(10)) v.push_back(x);
    EXPECT_EQ(v, (std::vector<int>{2,3,5,7,11,13,17,19,23,29}));
}

TEST(GeneratorTest, ExceptionPropagates) {
    auto gen = throwAfter(3);
    auto it = gen.begin();
    EXPECT_EQ(*it, 0);
    ++it; EXPECT_EQ(*it, 1);
    ++it; EXPECT_EQ(*it, 2);
    EXPECT_THROW(++it, std::runtime_error);
}

TEST(GeneratorTest, RecursiveInorder) {
    TreeNode n1{1}, n2{2}, n3{3};
    n2.left = &n1; n2.right = &n3;
    std::vector<int> v;
    for (const auto* n : inorder(&n2)) v.push_back(n->value);
    EXPECT_EQ(v, (std::vector<int>{1,2,3}));
}