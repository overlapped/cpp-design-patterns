#include <gtest/gtest.h>
#include "event_bus.hpp"
#include <string>
#include <utility>

struct EventA { int value; };
struct EventB { std::string name; };

TEST(EventBusTest, SubscribeAndPublish) {
    EventBus bus;
    int count = 0;
    auto sub = bus.subscribe<EventA>([&](const EventA&) { ++count; });
    bus.publish(EventA{1});
    bus.publish(EventA{2});
    EXPECT_EQ(count, 2);
}

TEST(EventBusTest, SubscriptionAutoUnsubscribesOnDestroy) {
    EventBus bus;
    int count = 0;
    {
        auto sub = bus.subscribe<EventA>([&](const EventA&) { ++count; });
        bus.publish(EventA{1});
        EXPECT_EQ(count, 1);
    }   // sub уничтожен — отписался
    bus.publish(EventA{2});
    EXPECT_EQ(count, 1);   // не изменилось
}

TEST(EventBusTest, ExplicitUnsubscribe) {
    EventBus bus;
    int count = 0;
    auto sub = bus.subscribe<EventA>([&](const EventA&) { ++count; });
    bus.publish(EventA{1});
    EXPECT_EQ(count, 1);
    sub.unsubscribe();
    bus.publish(EventA{2});
    EXPECT_EQ(count, 1);
}

TEST(EventBusTest, MoveSubscription) {
    EventBus bus;
    int count = 0;
    auto sub1 = bus.subscribe<EventA>([&](const EventA&) { ++count; });
    auto sub2 = std::move(sub1);
    EXPECT_FALSE(sub1.valid());
    EXPECT_TRUE(sub2.valid());
    bus.publish(EventA{1});
    EXPECT_EQ(count, 1);
}

TEST(EventBusTest, DifferentEventTypesAreIndependent) {
    EventBus bus;
    int aCount = 0;
    int bCount = 0;
    auto subA = bus.subscribe<EventA>([&](const EventA&) { ++aCount; });
    auto subB = bus.subscribe<EventB>([&](const EventB&) { ++bCount; });
    bus.publish(EventA{1});
    bus.publish(EventB{"x"});
    EXPECT_EQ(aCount, 1);
    EXPECT_EQ(bCount, 1);
    bus.publish(EventA{2});
    EXPECT_EQ(aCount, 2);
    EXPECT_EQ(bCount, 1);   // EventB не затронут
}

TEST(EventBusTest, UnsubscribeDuringPublishIsSafe) {
    EventBus bus;
    int count = 0;
    EventBus::Subscription sub;
    sub = bus.subscribe<EventA>([&](const EventA&) {
        ++count;
        sub.unsubscribe();   // отписываемся во время publish
    });
    bus.publish(EventA{1});
    EXPECT_EQ(count, 1);
    bus.publish(EventA{2});
    EXPECT_EQ(count, 1);   // больше не вызывается
}