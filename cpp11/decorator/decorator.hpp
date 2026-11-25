#pragma once
#include <memory>
#include <string>

class Coffee {
public:
    virtual ~Coffee() = default;
    virtual std::string description() const = 0;
    virtual int cost() const = 0;
};

class Espresso : public Coffee {
public:
    std::string description() const override { return "Espresso"; }
    int cost() const override { return 2; }
};

class CoffeeDecorator : public Coffee {
public:
    explicit CoffeeDecorator(std::unique_ptr<Coffee> c) : inner_(std::move(c)) {}
protected:
    std::unique_ptr<Coffee> inner_;
};

class Milk : public CoffeeDecorator {
public:
    using CoffeeDecorator::CoffeeDecorator;
    std::string description() const override { return inner_->description() + " + Milk"; }
    int cost() const override { return inner_->cost() + 1; }
};

class Sugar : public CoffeeDecorator {
public:
    using CoffeeDecorator::CoffeeDecorator;
    std::string description() const override { return inner_->description() + " + Sugar"; }
    int cost() const override { return inner_->cost() + 1; }
};