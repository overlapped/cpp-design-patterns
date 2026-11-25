#include <gtest/gtest.h>
#include "observer.hpp"

TEST(ObserverTest, NotifiesAll) {
    ConcreteSubject s;
    ConcreteObserver a, b;
    s.attach(&a);
    s.attach(&b);
    s.setState("x");
    EXPECT_EQ(a.events.size(), 1);
    EXPECT_EQ(b.events.size(), 1);
}

TEST(ObserverTest, DetachStopsNotifications) {
    ConcreteSubject s;
    ConcreteObserver a;
    s.attach(&a);
    s.detach(&a);
    s.setState("y");
    EXPECT_TRUE(a.events.empty());
}