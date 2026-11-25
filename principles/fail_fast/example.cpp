#include "fail_fast.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>

int main() {
    // Плохо: ошибка проглочена, баланс ушёл в минус
    bad::BankAccount b;
    b.deposit(-100);          // молча проигнорировано
    b.withdraw(50);           // баланс -50, никто не заметил
    assert(b.balance() == -50);

    // Хорошо: падаем сразу
    good::BankAccount g;
    bool threw = false;
    try { g.deposit(-100); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    threw = false;
    try { g.withdraw(1); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);

    g.deposit(100);
    g.withdraw(40);
    assert(g.balance() == 60);

    threw = false;
    try { good::Range r(10, 5); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    std::cout << "Fail Fast: invalid input rejected at the boundary\n";
    return 0;
}