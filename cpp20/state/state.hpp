#pragma once
#include <memory>

class Context;

class State {
public:
    virtual ~State() = default;
    virtual void handle(Context& ctx) = 0;
};

class Context {
public:
    void setState(std::unique_ptr<State> s) { state_ = std::move(s); }
    void request() { if (state_) state_->handle(*this); }
private:
    std::unique_ptr<State> state_;
};

class ConcreteStateA : public State {
public:
    void handle(Context& ctx) override;
};

class ConcreteStateB : public State {
public:
    void handle(Context& ctx) override;
};

inline void ConcreteStateA::handle(Context& ctx) {
    ctx.setState(std::make_unique<ConcreteStateB>());
}

inline void ConcreteStateB::handle(Context& ctx) {
    ctx.setState(std::make_unique<ConcreteStateA>());
}