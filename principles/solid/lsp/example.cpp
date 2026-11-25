#include "lsp.hpp"
#include <iostream>

int main() {
    // Демонстрация нарушения: код, ожидающий Rectangle, ломается на Square
    bad::Rectangle& r = *new bad::Square();
    r.setWidth(3); r.setHeight(4);
    // Ожидаем 12, получаем 16 — контракт нарушен
    std::cout << "bad::Square area = " << r.area() << " (expected 12)\n";
    delete &r;

    good::Rectangle gr(3, 4);
    good::Square    gs(3);
    std::cout << "good::Rectangle area = " << gr.area() << "\n";
    std::cout << "good::Square area = "    << gs.area() << "\n";
    return 0;
}