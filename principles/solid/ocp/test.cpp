#include <gtest/gtest.h>
#include "ocp.hpp"

TEST(OcpTest, CircleArea) {
    EXPECT_NEAR(bad::area({bad::ShapeType::Circle, 2.0}), 12.56636, 1e-4);
    std::vector<std::unique_ptr<good::Shape>> v;
    v.push_back(std::make_unique<good::Circle>(2.0));
    EXPECT_NEAR(good::totalArea(v), 12.56636, 1e-4);
}

TEST(OcpTest, ExtendWithNewShape) {
    // Новый класс — без правок good::totalArea
    class Triangle : public good::Shape {
    public:
        explicit Triangle(double b, double h) : b_(b), h_(h) {}
        double area() const override { return 0.5 * b_ * h_; }
    private:
        double b_, h_;
    };
    std::vector<std::unique_ptr<good::Shape>> v;
    v.push_back(std::make_unique<Triangle>(4.0, 3.0));
    EXPECT_NEAR(good::totalArea(v), 6.0, 1e-9);
}