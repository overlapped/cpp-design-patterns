#include "kiss.hpp"
#include <cassert>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4};

    // Плохой вариант: 3 класса + фабрика + утечка (если забыть delete)
    auto* p = bad::ProcessorFactory::create();
    int r1 = p->process(v);
    delete p;

    // Хороший вариант: одна строка
    int r2 = good::sum(v);

    assert(r1 == r2);
    std::cout << "KISS: both give " << r2 << "\n";
    return 0;
}