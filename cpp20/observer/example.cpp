#include "observer.hpp"
#include <cassert>

int main() {
    ConcreteSubject s;
    ConcreteObserver o;
    s.attach(&o);
    int cb_count = 0;
    auto t = s.subscribe([&](const std::string&) { ++cb_count; });
    s.setState("A");
    assert(o.events.size() == 1);
    assert(cb_count == 1);
    s.unsubscribe(t);
    s.setState("B");
    assert(cb_count == 1);
    assert(o.events.size() == 2);
    return 0;
}