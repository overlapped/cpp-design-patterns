#include "cqs.hpp"
#include <cassert>
#include <iostream>

int main() {
    // ❌ Плохо: getSize() неожиданно очищает стек
    bad::Stack bs;
    bs.push(1); bs.push(2);
    int n = bs.getSize();
    assert(n == 2);
    assert(bs.getSize() == 0);   // сюрприз: стек пуст

    // ❌ Плохо: pop() возвращает значение, но его нельзя
    // вызвать «просто так» — нужно принимать int
    bad::Stack bs2;
    bs2.push(42);
    int v = bs2.pop();
    assert(v == 42);

    // ✅ Хорошо: query — const, command — void
    good::Stack gs;
    gs.push(1);
    gs.push(2);
    assert(gs.size() == 2);        // query, состояние не меняется
    assert(gs.size() == 2);        // повторный вызов — тот же результат
    assert(gs.top() == 2);         // query
    gs.pop();                      // command, возвращает void
    assert(gs.size() == 1);        // query
    assert(gs.top() == 1);

    // Проверка «не пуст ли» — отдельный query, а не часть pop()
    assert(!gs.empty());
    gs.pop();
    assert(gs.empty());
    assert(gs.top() == std::nullopt);

    std::cout << "CQS: queries const, commands void\n";
    return 0;
}