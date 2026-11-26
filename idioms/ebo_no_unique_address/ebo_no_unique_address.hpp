#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

// Кроссплатформенный макрос: MSVC требует [[msvc::no_unique_address]] для ABI-совместимости
#if defined(_MSC_VER) && !defined(__clang__)
#define IDIOMS_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define IDIOMS_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

namespace idioms {

// Пустой функтор-стратегия (stateless policy), например, Deleter или Allocator
struct DefaultDeleter {
    template <typename T>
    void operator()(T* ptr) const noexcept {
        delete ptr;
    }
};

// Финальный пустой класс: от него НЕЛЬЗЯ унаследоваться (EBO не сработает!)
struct FinalDeleter final {
    template <typename T>
    void operator()(T* ptr) const noexcept {
        delete ptr;
    }
};

// ============================================================================
// 1. Наивная реализация (БЕЗ оптимизации)
// ============================================================================
// В C++ любой отдельный объект (даже пустая структура) обязан иметь размер >= 1 байт,
// чтобы у двух разных объектов были уникальные адреса в памяти.
// Из-за выравнивания указателя T* (8 байт) 1 байт превращается в 8 байт потерь.
template <typename T, typename Deleter = DefaultDeleter>
class NaivePtr {
public:
    explicit NaivePtr(T* ptr = nullptr, Deleter d = Deleter{}) noexcept
        : ptr_(ptr), deleter_(std::move(d)) {}

    ~NaivePtr() {
        if (ptr_) {
            deleter_(ptr_);
        }
    }

    NaivePtr(const NaivePtr&) = delete;
    NaivePtr& operator=(const NaivePtr&) = delete;

    [[nodiscard]] T* get() const noexcept { return ptr_; }

private:
    T* ptr_;
    Deleter deleter_; // Занимает 1 байт + 7 байт padding на 64-битных системах!
};

// ============================================================================
// 2. Empty Base Optimization — EBO (C++11)
// ============================================================================
// Стандарт C++ разрешает базовому подобъекту без нестатических полей (empty base)
// иметь размер 0 байт и разделять адрес с первым полем производного класса.
// Ограничение: не работает, если Deleter объявлен как `final` или не является классом.
template <typename T, typename Deleter,
          bool UseEbo = std::is_empty_v<Deleter> && !std::is_final_v<Deleter>>
class CompressedStorage;

// Специализация с EBO (наследуемся от Deleter)
template <typename T, typename Deleter>
class CompressedStorage<T, Deleter, true> : private Deleter {
public:
    explicit CompressedStorage(T* ptr, Deleter d) noexcept
        : Deleter(std::move(d)), ptr_(ptr) {}

    [[nodiscard]] T*& ptr() noexcept { return ptr_; }
    [[nodiscard]] T* ptr() const noexcept { return ptr_; }
    [[nodiscard]] Deleter& deleter() noexcept { return *this; }

private:
    T* ptr_;
};

// Fallback-специализация (если Deleter не пустой или final)
template <typename T, typename Deleter>
class CompressedStorage<T, Deleter, false> {
public:
    explicit CompressedStorage(T* ptr, Deleter d) noexcept
        : ptr_(ptr), deleter_(std::move(d)) {}

    [[nodiscard]] T*& ptr() noexcept { return ptr_; }
    [[nodiscard]] T* ptr() const noexcept { return ptr_; }
    [[nodiscard]] Deleter& deleter() noexcept { return deleter_; }

private:
    T* ptr_;
    Deleter deleter_;
};

template <typename T, typename Deleter = DefaultDeleter>
class EboPtr {
public:
    explicit EboPtr(T* ptr = nullptr, Deleter d = Deleter{}) noexcept
        : storage_(ptr, std::move(d)) {}

    ~EboPtr() {
        if (storage_.ptr()) {
            storage_.deleter()(storage_.ptr());
        }
    }

    EboPtr(const EboPtr&) = delete;
    EboPtr& operator=(const EboPtr&) = delete;

    [[nodiscard]] T* get() const noexcept { return storage_.ptr(); }

private:
    CompressedStorage<T, Deleter> storage_;
};

// ============================================================================
// 3. Атрибут [[no_unique_address]] (C++20)
// ============================================================================
// Позволяет компилятору не выделять отдельную память под пустое поле класса,
// накладывая его на адрес других полей — без наследования и шаблонных трюков.
// Бонус: отлично работает даже с `final` классами!
template <typename T, typename Deleter = DefaultDeleter>
class ModernPtr {
public:
    explicit ModernPtr(T* ptr = nullptr, Deleter d = Deleter{}) noexcept
        : ptr_(ptr), deleter_(std::move(d)) {}

    ~ModernPtr() {
        if (ptr_) {
            deleter_(ptr_);
        }
    }

    ModernPtr(const ModernPtr&) = delete;
    ModernPtr& operator=(const ModernPtr&) = delete;

    [[nodiscard]] T* get() const noexcept { return ptr_; }

private:
    T* ptr_;
    IDIOMS_NO_UNIQUE_ADDRESS Deleter deleter_;
};

} // namespace idioms
