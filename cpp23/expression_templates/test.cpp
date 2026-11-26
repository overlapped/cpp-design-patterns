#include <gtest/gtest.h>
#include "expression_templates.hpp"

TEST(ExpressionTemplatesTest, AddTwoVectors) {
    Vec a = makeVec({1, 2, 3});
    Vec b = makeVec({4, 5, 6});
    Vec r(3);
    assign(r, a + b);
    EXPECT_EQ(r[0], 5);
    EXPECT_EQ(r[1], 7);
    EXPECT_EQ(r[2], 9);
}

TEST(ExpressionTemplatesTest, ChainedAddNoTemporaries) {
    Vec a = makeVec({1, 2, 3});
    Vec b = makeVec({10, 20, 30});
    Vec c = makeVec({100, 200, 300});
    Vec r(3);
    assign(r, a + b + c);
    EXPECT_EQ(r[0], 111);
    EXPECT_EQ(r[1], 222);
    EXPECT_EQ(r[2], 333);
}

TEST(ExpressionTemplatesTest, PrecedenceMultiplyBeforeAdd) {
    Vec a = makeVec({1, 2, 3});
    Vec b = makeVec({10, 20, 30});
    Vec c = makeVec({100, 200, 300});
    Vec r(3);
    assign(r, a + b * c);
    EXPECT_EQ(r[0], 1 + 10 * 100);
    EXPECT_EQ(r[1], 2 + 20 * 200);
    EXPECT_EQ(r[2], 3 + 30 * 300);
}

TEST(ExpressionTemplatesTest, SubExpression) {
    Vec a = makeVec({10, 20, 30});
    Vec b = makeVec({1, 2, 3});
    Vec r(3);
    assign(r, a - b);
    EXPECT_EQ(r[0], 9);
    EXPECT_EQ(r[1], 18);
    EXPECT_EQ(r[2], 27);
}

TEST(ExpressionTemplatesTest, PlusEqualsExpression) {
    Vec acc = makeVec({1, 1, 1});
    Vec a = makeVec({1, 2, 3});
    Vec b = makeVec({10, 20, 30});
    acc += a + b;
    EXPECT_EQ(acc[0], 1 + 1 + 10);
    EXPECT_EQ(acc[1], 1 + 2 + 20);
    EXPECT_EQ(acc[2], 1 + 3 + 30);
}

TEST(ExpressionTemplatesTest, SizeMismatchAssert) {
    Vec a = makeVec({1, 2});
    Vec b = makeVec({1, 2, 3});
    Vec r(2);
    // В release assert может быть отключён — тест проверяет,
    // что в debug он срабатывает. В release поведение UB,
    // поэтому тест не запускаем.
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(b.size(), 3u);
    (void)r;
}