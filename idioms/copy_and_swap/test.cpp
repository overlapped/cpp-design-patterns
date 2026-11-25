#include <gtest/gtest.h>
#include "copy_and_swap.hpp"
#include <cstring>
#include <utility>

TEST(CopyAndSwapTest, CopyAssignment) {
    String a("abc");
    String b;
    b = a;
    EXPECT_STREQ(b.c_str(), "abc");
    EXPECT_STREQ(a.c_str(), "abc");
    EXPECT_EQ(b.size(), 3u);
}

TEST(CopyAndSwapTest, SelfAssignment) {
    String a("xyz");
    a = a;
    EXPECT_STREQ(a.c_str(), "xyz");
    EXPECT_EQ(a.size(), 3u);
}

TEST(CopyAndSwapTest, MoveAssignment) {
    String a("move");
    String b;
    b = std::move(a);
    EXPECT_STREQ(b.c_str(), "move");
    EXPECT_EQ(b.size(), 4u);
}

TEST(CopyAndSwapTest, SwapFunction) {
    String a("one");
    String b("two");
    swap(a, b);
    EXPECT_STREQ(a.c_str(), "two");
    EXPECT_STREQ(b.c_str(), "one");
}