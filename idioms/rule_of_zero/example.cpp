#include "rule_of_zero.hpp"
#include <cassert>
#include <iostream>
#include <utility>

int main() {
    // Document: копируется и перемещается, хотя специальных членов нет
    Document a("Report");
    a.addLine("first");
    a.addLine("second");

    Document b = a;              // копия — независимые данные
    b.addLine("third");
    assert(a.lines().size() == 2);
    assert(b.lines().size() == 3);

    Document c = std::move(a);   // перемещение — источник опустошён
    assert(c.lines().size() == 2);
    // a после move валиден, но его содержимое неопределено —
    // не проверяем size(), чтобы не зависеть от реализации.

    // Widget: move-only, копирование запрещено автоматически
    Widget w1(42);
    Widget w2 = std::move(w1);
    assert(w2.value() == 42);

    // Widget w3 = w2;  // ошибка компиляции: копирование удалено

    std::cout << "Rule of Zero: no special members written by hand\n";
    return 0;
}