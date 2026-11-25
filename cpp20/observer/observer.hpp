#pragma once
#include <algorithm>
#include <functional>
#include <string>
#include <utility>
#include <vector>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& event) = 0;
};

class Subject {
public:
    using Callback = std::function<void(const std::string&)>;
    using Token = std::size_t;

    void attach(Observer* o) { observers_.push_back(o); }
    void detach(Observer* o) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), o),
                         observers_.end());
    }

    Token subscribe(Callback cb) {
        Token t = next_++;
        callbacks_.emplace_back(t, std::move(cb));
        return t;
    }
    void unsubscribe(Token t) {
        callbacks_.erase(
            std::remove_if(callbacks_.begin(), callbacks_.end(),
                           [t](const auto& p) { return p.first == t; }),
            callbacks_.end());
    }

    void notify(const std::string& event) {
        for (auto* o : observers_) o->update(event);
        for (auto& [_, cb] : callbacks_) cb(event);
    }
private:
    std::vector<Observer*> observers_;
    std::vector<std::pair<Token, Callback>> callbacks_;
    Token next_ = 0;
};

class ConcreteSubject : public Subject {
public:
    void setState(const std::string& s) { state_ = s; notify(s); }
    const std::string& state() const { return state_; }
private:
    std::string state_;
};

class ConcreteObserver : public Observer {
public:
    std::vector<std::string> events;
    void update(const std::string& e) override { events.push_back(e); }
};