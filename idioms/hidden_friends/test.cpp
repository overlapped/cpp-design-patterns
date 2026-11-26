#include "hidden_friends.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <type_traits>

TEST(HiddenFriendsTest, ArithmeticAndStreamOperatorsWorkViaAdl) {
    idioms::ModernNumber a{15};
    idioms::ModernNumber b{27};

    idioms::ModernNumber sum = a + b;
    EXPECT_EQ(sum, idioms::ModernNumber{42});

    // Симметричное преобразование работает для обоих операндов
    EXPECT_EQ(10 + a, idioms::ModernNumber{25});
    EXPECT_EQ(a + 10, idioms::ModernNumber{25});

    std::ostringstream oss;
    oss << sum;
    EXPECT_EQ(oss.str(), "ModernNumber(42)");
}

TEST(HiddenFriendsTest, StrictIdComparison) {
    idioms::StrictId id1{7};
    idioms::StrictId id2{7};
    idioms::StrictId id3{8};

    EXPECT_TRUE(id1 == id2);
    EXPECT_TRUE(id1 != id3);
    EXPECT_FALSE(id1 == id3);
}

TEST(HiddenFriendsTest, ConceptDetectsAdlOperator) {
    static_assert(idioms::HasAdlAddition<idioms::ModernNumber>,
                  "ModernNumber must support operator+ via ADL");
    static_assert(!idioms::HasAdlAddition<idioms::StrictId>,
                  "StrictId must not support operator+");
}
