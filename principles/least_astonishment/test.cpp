#include <gtest/gtest.h>
#include "least_astonishment.hpp"

TEST(LeastAstonishmentTest, BadGetSizeMutates) {
    bad::Container c;
    c.add(1); c.add(2);
    EXPECT_EQ(c.getSize(), 2);
    EXPECT_EQ(c.getSize(), 0);   // неожиданный побочный эффект
}

TEST(LeastAstonishmentTest, GoodSizeIsConstGetter) {
    good::Container c;
    c.add(1); c.add(2);
    EXPECT_EQ(c.size(), 2);
    EXPECT_EQ(c.size(), 2);      // повторный вызов ничего не меняет
}

TEST(LeastAstonishmentTest, TakeAllIsExplicit) {
    good::Container c;
    c.add(1); c.add(2);
    auto v = c.takeAll();
    EXPECT_EQ(v, (std::vector<int>{1, 2}));
    EXPECT_EQ(c.size(), 0);
}

TEST(LeastAstonishmentTest, GoodOperatorPlusAdds) {
    good::Vec2 a{3, 3}, b{1, 1};
    auto r = a + b;
    EXPECT_EQ(r.x, 4);
    EXPECT_EQ(r.y, 4);
}

TEST(LeastAstonishmentTest, BadOperatorPlusSurprises) {
    bad::Vec2 a{3, 3}, b{1, 1};
    auto r = a + b;
    EXPECT_EQ(r.x, 2);   // «+», но вычитание — нарушение ожиданий
    EXPECT_EQ(r.y, 2);
}