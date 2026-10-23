#pragma once
#include <memory>

class Handler {
public:
    virtual ~Handler() = default;
    void setNext(std::unique_ptr<Handler> next) { next_ = std::move(next); }
    virtual void handle(int request) {
        if (next_) next_->handle(request);
    }
protected:
    std::unique_ptr<Handler> next_;
};

class ConcreteHandlerA : public Handler {
public:
    void handle(int request) override {
        if (request < 10) { /* handled */ }
        else Handler::handle(request);
    }
};