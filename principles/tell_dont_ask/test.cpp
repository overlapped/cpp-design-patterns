#include <gtest/gtest.h>
#include "tell_dont_ask.hpp"
#include <stdexcept>

TEST(TellDontAskTest, BadVersionClientChecks) {
    bad::Account acc(100);
    bad::withdraw(acc, 30);
    EXPECT_EQ(acc.balance(), 70);
    EXPECT_THROW(bad::withdraw(acc, 1000), std::runtime_error);
}

TEST(TellDontAskTest, GoodVersionObjectChecks) {
    good::Account acc(100);
    acc.withdraw(30);
    EXPECT_EQ(acc.balance(), 70);
    EXPECT_THROW(acc.withdraw(1000), std::runtime_error);
}

TEST(TellDontAskTest, GoodVersionRejectsNegative) {
    good::Account acc(100);
    EXPECT_THROW(acc.withdraw(-10), std::invalid_argument);
    EXPECT_THROW(acc.deposit(0), std::invalid_argument);
    EXPECT_EQ(acc.balance(), 100);
}

TEST(TellDontAskTest, GoodVersionDeposit) {
    good::Account acc(100);
    acc.deposit(50);
    EXPECT_EQ(acc.balance(), 150);
}

TEST(TellDontAskTest, Bad2ClientChecksActive) {
    bad2::User u(true);
    EXPECT_TRUE(u.isActive());
    EXPECT_NO_THROW(u.sendEmail("hi"));
}

TEST(TellDontAskTest, Good2ObjectChecksActive) {
    good2::User active(true);
    EXPECT_NO_THROW(active.sendEmail("hi"));

    good2::User inactive(false);
    EXPECT_THROW(inactive.sendEmail("hi"), std::runtime_error);
}