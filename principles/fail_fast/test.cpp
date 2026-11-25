#include <gtest/gtest.h>
#include "fail_fast.hpp"
#include <stdexcept>

TEST(FailFastTest, BadVersionSilentlyCorrupts) {
    bad::BankAccount b;
    b.deposit(-100);          // ошибка не видна
    b.withdraw(50);
    EXPECT_EQ(b.balance(), -50);   // невалидное состояние
}

TEST(FailFastTest, GoodVersionThrowsOnBadDeposit) {
    good::BankAccount g;
    EXPECT_THROW(g.deposit(0), std::invalid_argument);
    EXPECT_THROW(g.deposit(-1), std::invalid_argument);
}

TEST(FailFastTest, GoodVersionThrowsOnOverdraft) {
    good::BankAccount g;
    EXPECT_THROW(g.withdraw(1), std::runtime_error);
    g.deposit(100);
    EXPECT_NO_THROW(g.withdraw(100));
    EXPECT_DOUBLE_EQ(g.balance(), 0);
}

TEST(FailFastTest, ConstructorRejectsInvalidRange) {
    EXPECT_THROW(good::Range(10, 5), std::invalid_argument);
    EXPECT_NO_THROW(good::Range(5, 10));
}

TEST(FailFastTest, ValidRangeWorks) {
    good::Range r(1, 3);
    EXPECT_TRUE(r.contains(2));
    EXPECT_FALSE(r.contains(4));
}