#include "visitor.hpp"
#include <string>
#include <vector>
#include <cassert>

class NamePrinter : public Visitor {
public:
    std::vector<std::string> names;
    void visit(ConcreteElementA&) override { names.push_back("A"); }
    void visit(ConcreteElementB&) override { names.push_back("B"); }
};

int main() {
    ConcreteElementA a;
    ConcreteElementB b;
    NamePrinter printer;
    a.accept(printer);
    b.accept(printer);
    assert(printer.names.size() == 2);
    return 0;
}