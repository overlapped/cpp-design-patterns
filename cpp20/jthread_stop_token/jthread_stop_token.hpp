#pragma once
#include <thread>
#include <stop_token>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <optional>

// std::jthread + std::stop_token (C++20): кооперативная остановка.
// jthread при уничтожении вызывает request_stop() и join().
// Поток получает stop_token и сам решает, когда завершиться.
//
// Это безопасная альтернатива std::thread + std::atomic<bool> флагу:
// - jthread автоматически join'ит (нет std::terminate);
// - stop_token интегрирован с condition_variable_any (wake on stop);
// - stop_source позволяет остановить несколько потоков сразу.

// 1. Простой worker: крутится, пока не попросят остановиться
class SimpleWorker {
public:
    SimpleWorker() : thread_([this](std::stop_token st) {
        while (!st.stop_requested()) {
            ++counter_;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }) {}

    ~SimpleWorker() = default;  // jthread сам сделает request_stop + join

    int counter() const { return counter_.load(); }

private:
    std::atomic<int> counter_{0};
    std::jthread thread_;
};

// 2. Worker с очередью: ждёт задачи, но просыпается по stop
class QueueWorker {
public:
    QueueWorker() : thread_([this](std::stop_token st) {
        while (!st.stop_requested()) {
            std::unique_lock lock(mtx_);
            // condition_variable_any::wait с stop_token:
            // просыпается и по notify, и по stop_requested()
            cv_.wait(lock, st, [this] { return !queue_.empty(); });

            if (st.stop_requested()) break;
            if (queue_.empty()) continue;

            int task = queue_.front();
            queue_.pop();
            lock.unlock();
            ++processed_;
        }
    }) {}

    ~QueueWorker() = default;

    void push(int task) {
        {
            std::lock_guard lock(mtx_);
            queue_.push(task);
        }
        cv_.notify_one();
    }

    int processed() const { return processed_.load(); }

private:
    std::mutex mtx_;
    std::condition_variable_any cv_;
    std::queue<int> queue_;
    std::atomic<int> processed_{0};
    std::jthread thread_;
};

// 3. Общий stop_source для нескольких потоков
class ParallelWorkers {
public:
    explicit ParallelWorkers(int n) {
        for (int i = 0; i < n; ++i) {
            workers_.emplace_back([this, i](std::stop_token st) {
                while (!st.stop_requested()) {
                    ++counters_[i];
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            });
        }
    }

    // Остановить всех сразу (не дожидаясь деструктора)
    void stopAll() {
        for (auto& w : workers_) w.request_stop();
    }

    int counter(int i) const { return counters_[i].load(); }

private:
    std::vector<std::atomic<int>> counters_{std::vector<std::atomic<int>>(8)};
    std::vector<std::jthread> workers_;
};