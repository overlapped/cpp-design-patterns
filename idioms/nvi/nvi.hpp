#pragma once
#include <string>

// NVI: публичный невиртуальный метод фиксирует контракт и вызывает
// приватный виртуальный. Клиент не может переопределить публичный метод,
// но наследники реализуют только «сырое» поведение.
class Shape {
public:
    virtual ~Shape() = default;

    // Публичный невиртуальный интерфейс
    std::string describe() const {
        // Общая логика до/после — нельзя случайно обойти наследникам
        std::string s = "Shape(";
        s += name();        // приватный виртуальный
        s += ", area=";
        s += std::to_string(area());
        s += ")";
        return s;
    }

    // Невиртуальный «шаблонный» метод (по сути Template Method на NVI)
    double scaledArea(double factor) const {
        if (factor < 0) return 0.0;
        return area() * factor;
    }

protected:
    // Приватный виртуальный: наследник обязан реализовать, но не может
    // переопределить публичный describe()/scaledArea().
    virtual std::string name() const = 0;
    virtual double area() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double r) : radius_(r) {}
protected:
    std::string name() const override { return "Circle"; }
    double area() const override { return 3.141592653589793 * radius_ * radius_; }
private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
protected:
    std::string name() const override { return "Rectangle"; }
    double area() const override { return w_ * h_; }
private:
    double w_, h_;
};