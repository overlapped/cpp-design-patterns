#include <gtest/gtest.h>
#include "yagni.hpp"

TEST(YagniTest, MinimalReportBehavesSame) {
    bad::Report  b;
    good::Report g;
    for (int i = 0; i < 5; ++i) { b.addLine("x"); g.addLine("x"); }
    EXPECT_EQ(b.lineCount(), g.lineCount());
}

TEST(YagniTest, EmptyReport) {
    good::Report g;
    EXPECT_EQ(g.lineCount(), 0);
}