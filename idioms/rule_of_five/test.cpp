#include <gtest/gtest.h>
#include "rule_of_five.hpp"
#include <utility>
#include <vector>

TEST(RuleOfFiveTest, CopyIsDeep) {
    Buffer a(2);
    a[0] = 1; a[1] = 2;
    Buffer b = a;
    b[0] = 42;
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(b[0], 42);
}

TEST(RuleOfFiveTest, MoveEmptiesSource) {
    Buffer a(3);
    a[0] = 7;
    Buffer b = std::move(a);
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 7);
}

TEST(RuleOfFiveTest, CopyAssignmentIsSafe) {
    Buffer a(2);
    a[0] = 1; a[1] = 2;
    Buffer b;
    b = a;
    EXPECT_EQ(b.size(), 2u);
    EXPECT_EQ(b[1], 2);
    // self-assignment
    b = b;
    EXPECT_EQ(b.size(), 2u);
    EXPECT_EQ(b[0], 1);
}

TEST(RuleOfFiveTest, MoveAssignmentEmptiesSource) {
    Buffer a(4);
    a[0] = 9;
    Buffer b;
    b = std::move(a);
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(b.size(), 4u);
    EXPECT_EQ(b[0], 9);
}

TEST(RuleOfFiveTest, StoredInVector) {
    std::vector<Buffer> v;
    v.reserve(2);
    v.emplace_back(2);
    v[0][0] = 5;
    v.emplace_back(1);
    v[1][0] = 6;
    EXPECT_EQ(v[0][0], 5);
    EXPECT_EQ(v[1][0], 6);
}