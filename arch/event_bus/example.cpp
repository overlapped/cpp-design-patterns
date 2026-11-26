#include "event_bus.hpp"
#include <cassert>
#include <iostream>
#include <string>

struct UserLoggedIn {
    std::string username;
};

struct OrderPlaced {
    int orderId;
    double amount;
};

int main() {
    EventBus bus;

    // 1. Подписка на UserLoggedIn
    int loginCount = 0;
    std::string lastUser;
    {
        auto sub = bus.subscribe<UserLoggedIn>(
            [&](const UserLoggedIn& e) {
                ++loginCount;
                lastUser = e.username;
            });

        bus.publish(UserLoggedIn{"alice"});
        bus.publish(UserLoggedIn{"bob"});
        assert(loginCount == 2);
        assert(lastUser == "bob");

        // sub уничтожается здесь — автоматически отписывается
    }

    // После уничтожения sub публикация не вызывает handler
    bus.publish(UserLoggedIn{"charlie"});
    assert(loginCount == 2);   // не изменилось

    // 2. Подписка на OrderPlaced
    double totalAmount = 0;
    auto sub2 = bus.subscribe<OrderPlaced>(
        [&](const OrderPlaced& e) { totalAmount += e.amount; });

    bus.publish(OrderPlaced{1, 100.0});
    bus.publish(OrderPlaced{2, 50.0});
    assert(totalAmount == 150.0);

    // 3. Явная отписка
    sub2.unsubscribe();
    bus.publish(OrderPlaced{3, 999.0});
    assert(totalAmount == 150.0);   // не изменилось

    // 4. Move Subscription
    int movedCount = 0;
    auto sub3 = bus.subscribe<UserLoggedIn>(
        [&](const UserLoggedIn&) { ++movedCount; });
    auto sub4 = std::move(sub3);   // sub3 больше не valid
    assert(!sub3.valid());
    assert(sub4.valid());
    bus.publish(UserLoggedIn{"dave"});
    assert(movedCount == 1);

    std::cout << "Event Bus: RAII subscription auto-unsubscribes\n";
    return 0;
}