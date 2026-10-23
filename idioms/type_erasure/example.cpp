#include "type_erasure.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

struct Circle {
    double r;
    void draw() const { std::cout << "Circle(" << r << ")\n"; }
};

struct Text {
    std::string s;
    void draw() const { std::cout << "Text(" << s << ")\n"; }
};

// Функция без наследования — работает с любым типом через AnyDrawable
void render(const AnyDrawable& d) { d.draw(); }

int main() {
    AnyDrawable a = Circle{2.0};
    AnyDrawable b = Text{"hello"};

    render(a);
    render(b);

    // Value-семантика: копирование независимо
    AnyDrawable c = a;
    render(c);

    // Контейнер разнородных типов
    std::vector<AnyDrawable> scene;
    scene.push_back(Circle{1.0});
    scene.push_back(Text{"world"});
    for (const auto& d : scene) d.draw();

    std::cout << "Type Erasure example OK\n";
    return 0;
}