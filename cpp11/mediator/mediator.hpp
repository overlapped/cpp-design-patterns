#pragma once
#include <string>
#include <vector>

class Colleague;

class Mediator {
public:
    virtual ~Mediator() = default;
    virtual void broadcast(Colleague* sender, const std::string& msg) = 0;
};

class Colleague {
public:
    explicit Colleague(Mediator* m) : mediator_(m) {}
    virtual ~Colleague() = default;
    void send(const std::string& msg) { mediator_->broadcast(this, msg); }
    virtual void receive(const std::string& msg) = 0;
protected:
    Mediator* mediator_;
};

class ChatMediator : public Mediator {
public:
    void add(Colleague* c) { colleagues_.push_back(c); }
    void broadcast(Colleague* sender, const std::string& msg) override {
        for (auto* c : colleagues_)
            if (c != sender) c->receive(msg);
    }
private:
    std::vector<Colleague*> colleagues_;
};

class User : public Colleague {
public:
    using Colleague::Colleague;
    std::vector<std::string> inbox;
    void receive(const std::string& msg) override { inbox.push_back(msg); }
};