#pragma once
#include <atomic>
#include <memory>
#include <mutex>

class Singleton {
public:
    static Singleton& instance() {
        static std::atomic<Singleton*> ptr{nullptr};
        static std::mutex mtx;
        Singleton* p = ptr.load(std::memory_order_acquire);
        if (!p) {
            std::lock_guard<std::mutex> lock(mtx);
            p = ptr.load(std::memory_order_relaxed);
            if (!p) {
                p = new Singleton();
                ptr.store(p, std::memory_order_release);
            }
        }
        return *p;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
private:
    Singleton() = default;
};