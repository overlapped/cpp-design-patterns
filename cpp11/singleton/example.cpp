#include "singleton.hpp"
#include <cassert>

int main() {
    Singleton& a = Singleton::instance();
    Singleton& b = Singleton::instance();
    a.doSomething();
    assert(&a == &b);
    return 0;
}