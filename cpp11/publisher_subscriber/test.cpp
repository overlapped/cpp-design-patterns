#include <gtest/gtest.h>
#include "publisher_subscriber.hpp"

TEST(PubSubTest, ReceivesMessages) {
    Publisher<int> pub;
    int sum = 0;
    pub.subscribe([&](int v) { sum += v; });
    pub.publish(1);
    pub.publish(2);
    EXPECT_EQ(sum, 3);
}

TEST(PubSubTest, Unsubscribe) {
    Publisher<int> pub;
    int count = 0;
    auto t = pub.subscribe([&](int) { ++count; });
    pub.publish(0);
    pub.unsubscribe(t);
    pub.publish(0);
    EXPECT_EQ(count, 1);
}