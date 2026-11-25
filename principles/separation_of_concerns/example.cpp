#include "separation_of_concerns.hpp"
#include <cassert>
#include <iostream>

int main() {
    // Плохо: не получится переиспользовать парсинг или сумму отдельно
    bad::processCsvLine("1,2,3");

    // Хорошо: каждую часть можно использовать и тестировать отдельно
    auto nums = good::parseCsv("1,2,3");
    good::validateNonNegative(nums);
    int s = good::sum(nums);
    std::cout << good::formatSum(s) << "\n";
    assert(s == 6);

    // Композиция
    assert(good::processCsvLine("4,5") == "sum=9");

    std::cout << "Separation of Concerns: parse / validate / compute / format\n";
    return 0;
}