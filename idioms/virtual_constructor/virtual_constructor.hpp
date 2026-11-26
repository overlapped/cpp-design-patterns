#pragma once
#include <memory>
#include <string>
#include <vector>

// Virtual Constructor (clone): в C++ нет виртуальных конструкторов,
// но есть виртуальный метод clone(), который возвращает копию
// объекта. Через базовый указатель можно копировать полиморфные
// объекты, не зная конкретного типа.

class Shape {
public:
    virtual ~Shape() = default;

    // Виртуальный «конструктор копирования»
    virtual std::unique_ptr<Shape> clone() const = 0;

    virtual double area() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}

    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Circle>(*this);
    }

    double area() const override { return 3.14159 * r_ * r_; }
    std::string name() const override { return "Circle"; }

private:
    double r_;
};

class Rectangle : public Shape {
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}

    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Rectangle>(*this);
    }

    double area() const override { return w_ * h_; }
    std::string name() const override { return "Rectangle"; }

private:
    double w_, h_;
};

// Копирование вектора полиморфных объектов через clone()
inline std::vector<std::unique_ptr<Shape>>
cloneAll(const std::vector<std::unique_ptr<Shape>>& shapes) {
    std::vector<std::unique_ptr<Shape>> out;
    out.reserve(shapes.size());
    for (const auto& s : shapes) {
        out.push_back(s->clone());
    }
    return out;
}