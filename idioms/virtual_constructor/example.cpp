#include "virtual_constructor.hpp"
#include <cassert>
#include <iostream>

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(2.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));

    // Копируем через базовый указатель, не зная конкретных типов
    auto copy = cloneAll(shapes);

    assert(copy.size() == shapes.size());
    assert(copy[0]->name() == "Circle");
    assert(copy[1]->name() == "Rectangle");
    assert(copy[0]->area() == shapes[0]->area());
    assert(copy[1]->area() == shapes[1]->area());

    // Копии независимы: меняем оригинал — копия не меняется
    shapes.clear();
    assert(copy[0]->name() == "Circle");

    std::cout << "Virtual Constructor: polymorphic clone works\n";
    return 0;
}