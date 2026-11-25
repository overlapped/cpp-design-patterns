#include <gtest/gtest.h>
#include "law_of_demeter.hpp"

TEST(LoDTest, BothVersionsAgree) {
    bad::Person bp;
    good::Person gp;
    EXPECT_DOUBLE_EQ(bad::getAmount(bp), good::getAmount(gp));
}

TEST(LoDTest, PersonExposesMoney) {
    good::Person p;
    EXPECT_DOUBLE_EQ(p.money(), 100.0);
    EXPECT_EQ(p.currencyCode(), "USD");
}

TEST(LoDTest, ClientTalksOnlyToPerson) {
    // Клиентский код в good::getAmount не знает про Wallet/Currency —
    // это и есть соблюдение LoD.
    good::Person p;
    EXPECT_NO_THROW((void)good::getAmount(p));
}