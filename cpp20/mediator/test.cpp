#include <gtest/gtest.h>
#include "mediator.hpp"

TEST(MediatorTest, BroadcastsToOthers) {
    ChatMediator chat;
    User a(&chat), b(&chat), c(&chat);
    chat.add(&a); chat.add(&b); chat.add(&c);
    a.send("hello");
    EXPECT_EQ(b.inbox.size(), 1);
    EXPECT_EQ(c.inbox.size(), 1);
    EXPECT_EQ(a.inbox.size(), 0);
}