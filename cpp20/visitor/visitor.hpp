#pragma once
#include <variant>
#include <string>

struct ConcreteElementA { std::string name = "A"; };
struct ConcreteElementB { int value = 0; };

using Element = std::variant<ConcreteElementA, ConcreteElementB>;

struct ConcreteVisitor {
    void operator()(const ConcreteElementA& a) const { (void)a; }
    void operator()(const ConcreteElementB& b) const { (void)b; }
};

inline void applyVisitor(const Element& e, const ConcreteVisitor& v) {
    std::visit(v, e);
}