#pragma once
#include <memory>
#include <string>

class Shape {
public:
    virtual ~Shape() = default;
    virtual std::unique_ptr<Shape> clone() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(int r = 0) : radius(r) {}
    std::unique_ptr<Shape> clone() const override {
        return std::unique_ptr<Shape>(new Circle(*this));
    }
    std::string name() const override { return "Circle"; }
    int radius;
};

class Square : public Shape {
public:
    explicit Square(int s = 0) : side(s) {}
    std::unique_ptr<Shape> clone() const override {
        return std::unique_ptr<Shape>(new Square(*this));
    }
    std::string name() const override { return "Square"; }
    int side;
};