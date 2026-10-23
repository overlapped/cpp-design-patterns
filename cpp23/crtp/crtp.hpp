#pragma once
#include <string>
#include <utility>

struct Printable {
    Printable() = default;

    void print(this auto const& self) {
        (void)self.to_string();
    }

    std::string describe(this auto const& self) {
        return "[" + self.to_string() + "]";
    }
};

struct Name : Printable {
    explicit Name(std::string v) : value(std::move(v)) {}
    std::string value;
    std::string to_string() const { return value; }
};

struct Point : Printable {
    Point(int x_, int y_) : x(x_), y(y_) {}
    int x, y;
    std::string to_string() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};