#pragma once
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

// Active Object: методы вызываются из клиентского потока, но исполняются
// в выделенном рабочем потоке через очередь запросов. Клиент получает
// std::future и может дождаться результата.
//
// Пример: "счётчик" с методами increment()/get(), которые безопасно
// вызываются из любого потока.
class ActiveCounter {
public:
    ActiveCounter() : worker_(&ActiveCounter::run, this) {}

    ~ActiveCounter() {
        {
            std::lock_guard<std::mutex> lk(mtx_);
            stop_ = true;
        }
        cv_.notify_all();
        if (worker_.joinable()) worker_.join();
    }

    ActiveCounter(const ActiveCounter&) = delete;
    ActiveCounter& operator=(const ActiveCounter&) = delete;

    // Асинхронный increment: возвращает future, чтобы клиент мог дождаться
    std::future<void> increment() {
        auto task = std::make_shared<std::packaged_task<void()>>(
            [this] { value_ += 1; });
        auto fut = task->get_future();
        enqueue([task] { (*task)(); });
        return fut;
    }

    // Синхронный get: клиент блокируется до результата
    std::future<int> get() {
        auto task = std::make_shared<std::packaged_task<int()>>(
            [this] { return value_; });
        auto fut = task->get_future();
        enqueue([task] { (*task)(); });
        return fut;
    }

private:
    using Task = std::function<void()>;

    void enqueue(Task t) {
        {
            std::lock_guard<std::mutex> lk(mtx_);
            tasks_.push(std::move(t));
        }
        cv_.notify_one();
    }

    void run() {
        for (;;) {
            Task task;
            {
                std::unique_lock<std::mutex> lk(mtx_);
                cv_.wait(lk, [this] { return stop_ || !tasks_.empty(); });
                if (stop_ && tasks_.empty()) return;
                task = std::move(tasks_.front());
                tasks_.pop();
            }
            task();
        }
    }

    std::thread worker_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<Task> tasks_;
    bool stop_ = false;
    int value_ = 0;   // доступ только из worker_-потока
};