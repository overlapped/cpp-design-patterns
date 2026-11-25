#pragma once
#include <algorithm>
#include <string>
#include <vector>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& event) = 0;
};

class Subject {
public:
    void attach(Observer* o) { observers_.push_back(o); }
    void detach(Observer* o) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), o),
                         observers_.end());
    }
    void notify(const std::string& event) {
        for (auto* o : observers_) o->update(event);
    }
private:
    std::vector<Observer*> observers_;
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