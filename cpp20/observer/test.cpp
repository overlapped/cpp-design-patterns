#include <gtest/gtest.h>
#include "observer.hpp"

TEST(ObserverTest, GoFAndCallback) {
    ConcreteSubject s;
    ConcreteObserver o;
    s.attach(&o);
    int n = 0;
    s.subscribe([&](const std::string&) { ++n; });
    s.setState("x");
    EXPECT_EQ(o.events.size(), 1);
    EXPECT_EQ(n, 1);
}

TEST(ObserverTest, Unsubscribe) {
    ConcreteSubject s;
    int n = 0;
    auto t = s.subscribe([&](const std::string&) { ++n; });
    s.setState("a");
    s.unsubscribe(t);
    s.setState("b");
    EXPECT_EQ(n, 1);
}