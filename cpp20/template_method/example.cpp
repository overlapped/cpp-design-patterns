#include "template_method.hpp"
#include <cassert>

class Counter : public AbstractClass<Counter> {
public:
    int count = 0;
    void step1() { ++count; }
    void step2() { ++count; }
    void step3() { ++count; }
};

int main() {
    Counter c;
    c.templateMethod();
    assert(c.count == 3);
    return 0;
}