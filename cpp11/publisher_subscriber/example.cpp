#include "publisher_subscriber.hpp"
#include <cassert>

int main() {
    Publisher<int> pub;
    int sum = 0;
    auto token = pub.subscribe([&](int v) { sum += v; });
    pub.publish(5);
    pub.publish(7);
    assert(sum == 12);
    pub.unsubscribe(token);
    pub.publish(100);
    assert(sum == 12);
    return 0;
}