#include <gtest/gtest.h>
#include "visitor.hpp"
#include <string>
#include <vector>

class Collector : public Visitor {
public:
    std::vector<std::string> seen;
    void visit(ConcreteElementA&) override { seen.push_back("A"); }
    void visit(ConcreteElementB&) override { seen.push_back("B"); }
};

TEST(VisitorTest, DispatchesCorrectly) {
    ConcreteElementA a;
    ConcreteElementB b;
    Collector c;
    a.accept(c);
    b.accept(c);
    EXPECT_EQ(c.seen, (std::vector<std::string>{"A", "B"}));
}