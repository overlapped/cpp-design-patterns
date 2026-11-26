#pragma once

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <optional>
#include <queue>
#include <stdexcept>
#include <utility>

namespace arch {

// Классический пример паттерна Monitor Object:
// потокобезопасная ограниченная очередь (Bounded Blocking Queue).
// Объект сам инкапсулирует мьютекс и условные переменные, предоставляя
// синхронизированный интерфейс без необходимости внешней блокировки.
template <typename T>
class BoundedMonitorQueue {
public:
    explicit BoundedMonitorQueue(std::size_t capacity) : capacity_(capacity) {
        if (capacity_ == 0) {
            throw std::invalid_argument("Capacity must be greater than zero");
        }
    }

    BoundedMonitorQueue(const BoundedMonitorQueue&) = delete;
    BoundedMonitorQueue& operator=(const BoundedMonitorQueue&) = delete;
    BoundedMonitorQueue(BoundedMonitorQueue&&) = delete;
    BoundedMonitorQueue& operator=(BoundedMonitorQueue&&) = delete;

    // Блокирующая вставка: ждёт освобождения места, если очередь заполнена.
    // Возвращает false, если очередь была закрыта (close()).
    bool push(T item) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [this] { return closed_ || queue_.size() < capacity_; });

        if (closed_) {
            return false;
        }

        queue_.push(std::move(item));
        not_empty_.notify_one();
        return true;
    }

    // Блокирующее извлечение: ждёт появления элемента.
    // Возвращает std::nullopt, если очередь закрыта и пуста.
    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { return closed_ || !queue_.empty(); });

        if (queue_.empty()) {
            return std::nullopt;
        }

        T item = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();
        return item;
    }

    // Попытка извлечения с таймаутом
    template <typename Rep, typename Period>
    std::optional<T> try_pop_for(const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!not_empty_.wait_for(lock, timeout, [this] { return closed_ || !queue_.empty(); })) {
            return std::nullopt;
        }

        if (queue_.empty()) {
            return std::nullopt;
        }

        T item = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();
        return item;
    }

    // Закрывает монитор и пробуждает все ожидающие потоки
    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }
        not_empty_.notify_all();
        not_full_.notify_all();
    }

    [[nodiscard]] bool is_closed() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return closed_;
    }

    [[nodiscard]] std::size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    [[nodiscard]] bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    [[nodiscard]] std::size_t capacity() const noexcept {
        return capacity_;
    }

private:
    const std::size_t capacity_;
    std::queue<T> queue_;

    mutable std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    bool closed_{false};
};

} // namespace arch
