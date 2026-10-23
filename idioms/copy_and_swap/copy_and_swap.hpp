#pragma once
#include <cstddef>
#include <utility>

// Copy-and-Swap: operator= реализуется через копирование + swap.
// Преимущества:
//   - строгая гарантия исключений (если копирование бросит, *this не тронут);
//   - корректный self-assignment «бесплатно»;
//   - один код для copy- и move-присваивания (если добавить move-версию swap).
//
// Цена: лишняя копия в цепочке. Для дешёвых типов это нормально;
// для дорогих иногда лучше писать отдельные operator=.

class String {
public:
    String() = default;

    explicit String(const char* s) {
        if (!s) return;
        std::size_t n = 0;
        while (s[n]) ++n;
        data_ = new char[n + 1];
        for (std::size_t i = 0; i <= n; ++i) data_[i] = s[i];
        size_ = n;
    }

    String(const String& other)
        : size_(other.size_),
          data_(other.size_ ? new char[other.size_ + 1] : nullptr) {
        for (std::size_t i = 0; i <= size_; ++i) data_[i] = other.data_[i];
    }

    String(String&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
    }

    // Ключевая часть: копия + swap
    String& operator=(String other) {   // принимаем по значению — копия уже сделана
        swap(*this, other);
        return *this;
    }
    // Благодаря "operator=(String other)" move-присваивание тоже работает:
    // при передаче rvalue конструктор перемещения создаст other, а swap заберёт данные.

    ~String() { delete[] data_; }

    friend void swap(String& a, String& b) noexcept {
        std::swap(a.size_, b.size_);
        std::swap(a.data_, b.data_);
    }

    std::size_t size() const { return size_; }
    const char* c_str() const { return data_ ? data_ : ""; }

private:
    std::size_t size_ = 0;
    char* data_ = nullptr;
};