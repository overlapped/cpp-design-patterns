#include "double_buffering.hpp"
#include <cassert>
#include <iostream>

int main() {
    DoubleBuffer<int, 4> db;
    assert(db.front_index() == 0);
    assert(db.back_index() == 1);

    // Пишем кадр 1 в back
    db.back() = {1, 2, 3, 4};
    // Пока не swap — front всё ещё пустой
    assert(db.front()[0] == 0);

    db.swap();
    // Теперь front — это кадр 1
    assert(db.front()[0] == 1);
    assert(db.front()[3] == 4);

    // Пишем кадр 2 в новый back
    db.back() = {10, 20, 30, 40};
    // front не изменился
    assert(db.front()[0] == 1);

    db.swap();
    assert(db.front()[0] == 10);

    std::cout << "Double Buffering example OK\n";
    return 0;
}