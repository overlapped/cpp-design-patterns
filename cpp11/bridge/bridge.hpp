#pragma once

class Implementor {
public:
    virtual ~Implementor() = default;
    virtual void operationImpl() = 0;
};

class ConcreteImplementorA : public Implementor {
public:
    void operationImpl() override {}
};

class Abstraction {
protected:
    Implementor* impl_;
public:
    explicit Abstraction(Implementor* impl) : impl_(impl) {}
    virtual ~Abstraction() = default;
    virtual void operation() { impl_->operationImpl(); }
};