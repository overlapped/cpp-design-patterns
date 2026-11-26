#include "type_erasure.hpp"

#include <iostream>
#include <string>
#include <vector>

// Обратите внимание: ни один из этих классов НЕ наследуется от общего базового интерфейса!
struct Circle {
    double radius;

    [[nodiscard]] std::string draw() const {
        return "Circle(r=" + std::to_string(radius) + ")";
    }

    [[nodiscard]] double area() const {
        return 3.1415926535 * radius * radius;
    }
};

struct Rectangle {
    double width;
    double height;

    [[nodiscard]] std::string draw() const {
        return "Rectangle(" + std::to_string(width) + "x" + std::to_string(height) + ")";
    }

    [[nodiscard]] double area() const {
        return width * height;
    }
};

// Крупный объект, который не поместится в SBO-буфер (32 байта) и уйдёт в кучу
struct LargePolygon {
    double vertices[16]{};
    double cached_area{42.0};

    [[nodiscard]] std::string draw() const { return "LargePolygon(16 vertices)"; }
    [[nodiscard]] double area() const { return cached_area; }
};

int main() {
    // Храним совершенно разные типы в одном контейнере по значению (без std::unique_ptr!)
    std::vector<idioms::Shape> canvas;
    canvas.emplace_back(Circle{5.0});
    canvas.emplace_back(Rectangle{4.0, 6.0});
    canvas.emplace_back(LargePolygon{});

    for (const auto& shape : canvas) {
        std::cout << shape.draw()
                  << " | area = " << shape.area()
                  << " | storage = " << (shape.is_stored_on_heap() ? "HEAP" : "SBO (stack)")
                  << '\n';
    }

    // Семантика значения: копирование создаёт независимую копию стёртого объекта
    idioms::Shape copy = canvas[0];
    std::cout << "\nCopied shape: " << copy.draw() << '\n';

    return 0;
}
