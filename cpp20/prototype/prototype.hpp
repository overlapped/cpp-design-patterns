#pragma once
#include <concepts>
#include <memory>
#include <string>

class Shape {
public:
    virtual ~Shape() = default;
    virtual std::unique_ptr<Shape> clone() const = 0;
    virtual std::string name() const = 0;
};

template<typename T>
concept Cloneable = requires(const T& t) {
    { t.clone() } -> std::same_as<std::unique_ptr<Shape>>;
};

class Circle : public Shape {
public:
    explicit Circle(int r = 0) : radius(r) {}
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Circle>(*this);
    }
    std::string name() const override { return "Circle"; }
    int radius;
};

class Square : public Shape {
public:
    explicit Square(int s = 0) : side(s) {}
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Square>(*this);
    }
    std::string name() const override { return "Square"; }
    int side;
};

static_assert(Cloneable<Circle>);
static_assert(Cloneable<Square>);