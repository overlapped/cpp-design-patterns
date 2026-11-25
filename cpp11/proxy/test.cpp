#include <gtest/gtest.h>
#include "proxy.hpp"

TEST(ProxyTest, LazyLoadsRealSubject) {
    LazyImageProxy p("dog.png");
    EXPECT_FALSE(p.loaded());
    EXPECT_EQ(p.display(), "real:dog.png");
    EXPECT_TRUE(p.loaded());
}