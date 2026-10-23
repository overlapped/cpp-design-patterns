#include <gtest/gtest.h>
#include "visitor.hpp"
#include <string>

struct Collect {
    std::string s;
    void operator()(const ConcreteElementA&) { s += "A"; }
    void operator()(const ConcreteElementB&) { s += "B"; }
};

TEST(VisitorTest, VariantVisit) {
    Element a = ConcreteElementA{};
    Element b = ConcreteElementB{};
    Collect c;
    std::visit(c, a);
    std::visit(c, b);
    EXPECT_EQ(c.s, "AB");
}