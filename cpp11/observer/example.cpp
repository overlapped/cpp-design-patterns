#include "observer.hpp"
#include <cassert>

int main() {
    ConcreteSubject s;
    ConcreteObserver o;
    s.attach(&o);
    s.setState("A");
    s.setState("B");
    assert(o.events.size() == 2);
    return 0;
}