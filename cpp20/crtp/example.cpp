#include "crtp.hpp"
#include <cassert>

class Counter : public Base<Counter> {
public:
    int count = 0;
    void implementation() { ++count; }
};

int main() {
    Counter c;
    c.interface();
    assert(c.count == 1);
    return 0;
}