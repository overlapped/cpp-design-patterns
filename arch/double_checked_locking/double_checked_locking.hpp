#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <utility>

namespace arch {

// Корректная реализация паттерна Double-Checked Locking (DCLP) в C++11+.
// Обеспечивает потокобезопасную ленивую инициализацию объекта без захвата мьютекса
// на каждом последующем обращении (fast path).
template <typename T>
class LazyDclp {
public:
    using Factory = std::function<std::unique_ptr<T>()>;

    explicit LazyDclp(Factory factory) : factory_(std::move(factory)) {}

    LazyDclp(const LazyDclp&) = delete;
    LazyDclp& operator=(const LazyDclp&) = delete;
    LazyDclp(LazyDclp&&) = delete;
    LazyDclp& operator=(LazyDclp&&) = delete;

    ~LazyDclp() {
        // В деструкторе внешняя конкуренция отсутствует, достаточно relaxed.
        T* ptr = instance_.load(std::memory_order_relaxed);
        delete ptr;
    }

    [[nodiscard]] T& get() {
        // 1-я проверка (без блокировки — Fast Path):
        // memory_order_acquire гарантирует, что все записи в память, сделанные
        // при конструировании объекта *ptr до store(release), будут видны текущему потоку.
        T* ptr = instance_.load(std::memory_order_acquire);

        if (ptr == nullptr) {
            std::lock_guard<std::mutex> lock(mutex_);

            // 2-я проверка (под блокировкой):
            // Другой поток мог успеть инициализировать объект, пока мы ждали мьютекс.
            // Внутри критической секции (под мьютексом) достаточно memory_order_relaxed.
            ptr = instance_.load(std::memory_order_relaxed);

            if (ptr == nullptr) {
                std::unique_ptr<T> created = factory_();
                ptr = created.get();

                // memory_order_release публикует полностью сконструированный объект:
                // компилятор и CPU не имеют права переупорядочить запись полей *ptr
                // ниже этой инструкции store.
                instance_.store(ptr, std::memory_order_release);
                created.release();
            }
        }

        return *ptr;
    }

    [[nodiscard]] bool is_initialized() const noexcept {
        return instance_.load(std::memory_order_acquire) != nullptr;
    }

private:
    Factory factory_;
    std::atomic<T*> instance_{nullptr};
    std::mutex mutex_;
};

// Современная альтернатива на базе std::call_once (для сравнения).
// Под капотом стандартная библиотека реализует тот же fast-path на атомиках.
template <typename T>
class LazyCallOnce {
public:
    using Factory = std::function<std::unique_ptr<T>()>;

    explicit LazyCallOnce(Factory factory) : factory_(std::move(factory)) {}

    LazyCallOnce(const LazyCallOnce&) = delete;
    LazyCallOnce& operator=(const LazyCallOnce&) = delete;

    [[nodiscard]] T& get() {
        std::call_once(flag_, [this]() { instance_ = factory_(); });
        return *instance_;
    }

    [[nodiscard]] bool is_initialized() const noexcept {
        return instance_ != nullptr;
    }

private:
    Factory factory_;
    std::once_flag flag_;
    std::unique_ptr<T> instance_;
};

} // namespace arch
