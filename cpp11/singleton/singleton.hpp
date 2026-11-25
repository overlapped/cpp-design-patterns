#pragma once

class Singleton {
public:
    static Singleton& instance() {
        static Singleton inst;
        return inst;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    void doSomething() {}
private:
    Singleton() = default;
};