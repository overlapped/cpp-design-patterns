#include <gtest/gtest.h>
#include "cqs.hpp"
#include <stdexcept>

TEST(CqsTest, BadGetSizeMutates) {
    bad::Stack s;
    s.push(1); s.push(2);
    EXPECT_EQ(s.getSize(), 2);
    EXPECT_EQ(s.getSize(), 0);   // неожиданный побочный эффект
}

TEST(CqsTest, BadPopReturnsValue) {
    bad::Stack s;
    s.push(42);
    EXPECT_EQ(s.pop(), 42);
}

TEST(CqsTest, GoodQueryDoesNotMutate) {
    good::Stack s;
    s.push(1); s.push(2);
    EXPECT_EQ(s.size(), 2u);
    EXPECT_EQ(s.size(), 2u);     // повторный query — тот же результат
    EXPECT_EQ(s.top(), 2);
    EXPECT_EQ(s.top(), 2);       // повторный query — тот же результат
}

TEST(CqsTest, GoodCommandReturnsVoid) {
    good::Stack s;
    s.push(1);
    s.pop();                     // возвращает void — нельзя случайно
                                 // использовать результат
    EXPECT_TRUE(s.empty());
}

TEST(CqsTest, PopOnEmptyThrows) {
    good::Stack s;
    EXPECT_THROW(s.pop(), std::runtime_error);
}

TEST(CqsTest, TopOnEmptyReturnsNullopt) {
    good::Stack s;
    EXPECT_EQ(s.top(), std::nullopt);
}