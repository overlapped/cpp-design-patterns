#include "ocp.hpp"
#include <cassert>
#include <iostream>

int main() {
    double badArea = bad::area({bad::ShapeType::Circle, 2.0});
    std::vector<std::unique_ptr<good::Shape>> v;
    v.push_back(std::make_unique<good::Circle>(2.0));
    assert(std::abs(badArea - good::totalArea(v)) < 1e-9);
    std::cout << "OCP: polymorphic extension matches\n";
    return 0;
}