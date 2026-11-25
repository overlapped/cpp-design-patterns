#pragma once

class Handler {
public:
    virtual ~Handler() = default;
    void setNext(Handler* next) { next_ = next; }
    virtual void handle(int request) {
        if (next_) next_->handle(request);
    }
protected:
    Handler* next_ = nullptr;
};

class ConcreteHandlerA : public Handler {
public:
    void handle(int request) override {
        if (request < 10) { /* handled */ }
        else Handler::handle(request);
    }
};