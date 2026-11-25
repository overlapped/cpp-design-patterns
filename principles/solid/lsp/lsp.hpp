#pragma once
#include <memory>
#include <cassert>

// ❌ Плохо: Square наследует Rectangle, но ломает контракт setWidth/setHeight
namespace bad {
    class Rectangle {
    public:
        virtual ~Rectangle() = default;
        virtual void setWidth(double w)  { w_ = w; }
        virtual void setHeight(double h) { h_ = h; }
        double width()  const { return w_; }
        double height() const { return h_; }
        double area()   const { return w_ * h_; }
    protected:
        double w_ = 0, h_ = 0;
    };

    class Square : public Rectangle {
    public:
        void setWidth(double w)  override { w_ = h_ = w; }
        void setHeight(double h) override { w_ = h_ = h; }
    };
}

// ✅ Хорошо: отдельные типы без «лживого» наследования
namespace good {
    class Shape {
    public:
        virtual ~Shape() = default;
        virtual double area() const = 0;
    };

    class Rectangle : public Shape {
    public:
        Rectangle(double w, double h) : w_(w), h_(h) {}
        double area() const override { return w_ * h_; }
    private:
        double w_, h_;
    };

    class Square : public Shape {
    public:
        explicit Square(double s) : s_(s) {}
        double area() const override { return s_ * s_; }
    private:
        double s_;
    };
}