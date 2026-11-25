#pragma once
#include <memory>
#include <vector>
#include <string>

// ❌ Плохо: чтобы добавить фигуру — правим функцию area() и enum
namespace bad {
    enum class ShapeType { Circle, Square };

    struct Shape {
        ShapeType type;
        double    size;
    };

    inline double area(const Shape& s) {
        switch (s.type) {
            case ShapeType::Circle: return 3.14159 * s.size * s.size;
            case ShapeType::Square: return s.size * s.size;
        }
        return 0;
    }
}

// ✅ Хорошо: новая фигура = новый класс, старый код не трогаем
namespace good {
    class Shape {
    public:
        virtual ~Shape() = default;
        virtual double area() const = 0;
    };

    class Circle : public Shape {
    public:
        explicit Circle(double r) : r_(r) {}
        double area() const override { return 3.14159 * r_ * r_; }
    private:
        double r_;
    };

    class Square : public Shape {
    public:
        explicit Square(double s) : s_(s) {}
        double area() const override { return s_ * s_; }
    private:
        double s_;
    };

    inline double totalArea(const std::vector<std::unique_ptr<Shape>>& shapes) {
        double sum = 0;
        for (auto& s : shapes) sum += s->area();
        return sum;
    }
}