#include <gtest/gtest.h>
#include "publisher_subscriber.hpp"
#include <string>

TEST(PubSubTest, StringMessages) {
    Publisher<std::string> pub;
    std::string got;
    pub.subscribe([&](std::string s) { got = std::move(s); });
    pub.publish("hi");
    EXPECT_EQ(got, "hi");
}