#pragma once
#include <memory>

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
    std::unique_ptr<Implementor> impl_;
public:
    explicit Abstraction(std::unique_ptr<Implementor> impl) : impl_(std::move(impl)) {}
    virtual ~Abstraction() = default;
    virtual void operation() { impl_->operationImpl(); }
};