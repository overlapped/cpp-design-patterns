#pragma once
#include <string>
#include <sstream>

struct Pizza {
    std::string dough;
    std::string sauce;
    std::string topping;
    std::string describe() const {
        std::ostringstream os;
        os << dough << " + " << sauce << " + " << topping;
        return os.str();
    }
};

class PizzaBuilder {
public:
    virtual ~PizzaBuilder() = default;
    void build() {
        dough_ = makeDough();
        sauce_ = makeSauce();
        topping_ = makeTopping();
    }
    Pizza get() const { return {dough_, sauce_, topping_}; }
protected:
    virtual std::string makeDough() = 0;
    virtual std::string makeSauce() = 0;
    virtual std::string makeTopping() = 0;
private:
    std::string dough_, sauce_, topping_;
};

class MargheritaBuilder : public PizzaBuilder {
protected:
    std::string makeDough() override { return "thin"; }
    std::string makeSauce() override { return "tomato"; }
    std::string makeTopping() override { return "mozzarella"; }
};

class PepperoniBuilder : public PizzaBuilder {
protected:
    std::string makeDough() override { return "thick"; }
    std::string makeSauce() override { return "tomato"; }
    std::string makeTopping() override { return "pepperoni"; }
};

class Waiter {
public:
    explicit Waiter(PizzaBuilder* b) : builder_(b) {}
    Pizza construct() {
        builder_->build();
        return builder_->get();
    }
private:
    PizzaBuilder* builder_;
};