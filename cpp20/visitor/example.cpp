#include "visitor.hpp"
#include <cassert>
#include <string>

struct NamesCollector {
    std::string out;
    void operator()(const ConcreteElementA&) { out += "A"; }
    void operator()(const ConcreteElementB&) { out += "B"; }
};

int main() {
    Element e1 = ConcreteElementA{};
    Element e2 = ConcreteElementB{};
    NamesCollector c;
    std::visit(c, e1);
    std::visit(c, e2);
    assert(c.out == "AB");
    return 0;
}