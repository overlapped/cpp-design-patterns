#include "least_astonishment.hpp"
#include <cassert>
#include <iostream>

int main() {
    // Плохо: getSize() «неожиданно» очищает контейнер
    bad::Container bc;
    bc.add(1); bc.add(2);
    int n = bc.getSize();
    assert(n == 2);
    assert(bc.getSize() == 0);   // сюрприз!

    // Хорошо: size() — честный геттер, takeAll() — явно «забирает»
    good::Container gc;
    gc.add(1); gc.add(2);
    assert(gc.size() == 2);
    auto all = gc.takeAll();
    assert(all.size() == 2);
    assert(gc.size() == 0);      // ожидаемо: взяли всё

    // Плохо: operator+ вычитает
    bad::Vec2 bv{3, 3};
    auto br = bv + bad::Vec2{1, 1};
    assert(br.x == 2 && br.y == 2);   // «+», а результат — разность

    // Хорошо: operator+ складывает
    good::Vec2 gv{3, 3};
    auto gr = gv + good::Vec2{1, 1};
    assert(gr.x == 4 && gr.y == 4);

    std::cout << "Least Astonishment: names match behavior\n";
    return 0;
}