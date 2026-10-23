#pragma once

class ConcreteElementB;
class ConcreteElementA;

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(ConcreteElementA&) = 0;
    virtual void visit(ConcreteElementB&) = 0;
};

class Element {
public:
    virtual ~Element() = default;
    virtual void accept(Visitor&) = 0;
};

class ConcreteElementA : public Element {
public:
    void accept(Visitor& v) override { v.visit(*this); }
};

class ConcreteElementB : public Element {
public:
    void accept(Visitor& v) override { v.visit(*this); }
};

class ConcreteVisitor : public Visitor {
public:
    void visit(ConcreteElementA&) override {}
    void visit(ConcreteElementB&) override {}
};