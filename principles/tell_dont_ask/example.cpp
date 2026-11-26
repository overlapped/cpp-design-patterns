#include "tell_dont_ask.hpp"
#include <cassert>
#include <iostream>

int main() {
    // ❌ Плохо: клиент сам проверяет баланс и вызывает setBalance
    bad::Account ba(100);
    bad::withdraw(ba, 30);
    assert(ba.balance() == 70);

    bool threw = false;
    try { bad::withdraw(ba, 1000); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);

    // ✅ Хорошо: объект сам проверяет инвариант
    good::Account ga(100);
    ga.withdraw(30);
    assert(ga.balance() == 70);

    threw = false;
    try { ga.withdraw(1000); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);

    // ❌ Плохо: клиент спрашивает isActive() и сам решает
    bad2::User bu(true);
    if (bu.isActive()) bu.sendEmail("hello");

    // ✅ Хорошо: объект сам решает
    good2::User gu(true);
    gu.sendEmail("hello");

    std::cout << "Tell, Don't Ask: object owns its invariants\n";
    return 0;
}