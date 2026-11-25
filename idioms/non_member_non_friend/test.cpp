#include <gtest/gtest.h>
#include "non_member_non_friend.hpp"
#include <sstream>

TEST(NonMemberTest, Addition) {
    EXPECT_EQ((Money(10, "EUR") + Money(5, "EUR")).amount(), 15);
}

TEST(NonMemberTest, Equality) {
    EXPECT_EQ(Money(1, "USD"), Money(1, "USD"));
    EXPECT_NE(Money(1, "USD"), Money(1, "EUR"));
    EXPECT_NE(Money(1, "USD"), Money(2, "USD"));
}

TEST(NonMemberTest, StreamOutput) {
    std::ostringstream os;
    os << Money(42, "GBP");
    EXPECT_EQ(os.str(), "42 GBP");
}

TEST(NonMemberTest, MemberPlusEquals) {
    Money a(10, "USD");
    a += Money(5, "USD");
    EXPECT_EQ(a.amount(), 15);
}