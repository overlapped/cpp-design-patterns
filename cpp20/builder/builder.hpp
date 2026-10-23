#pragma once
#include <string>
#include <utility>

struct Pizza {
    std::string dough;
    std::string sauce;
    std::string topping;
};

class PizzaBuilder {
public:
    PizzaBuilder& dough(std::string d) { dough_ = std::move(d); return *this; }
    PizzaBuilder& sauce(std::string s) { sauce_ = std::move(s); return *this; }
    PizzaBuilder& topping(std::string t) { topping_ = std::move(t); return *this; }
    Pizza build() const { return {dough_, sauce_, topping_}; }
private:
    std::string dough_, sauce_, topping_;
};