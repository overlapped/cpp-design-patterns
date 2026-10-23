#pragma once
#include <cstddef>
#include <utility>

// Демонстрация «Rule of Five»: если классу нужен хотя бы один из
// деструктора / копирования / перемещения — объявите (или удалите) все пять.
//
// Здесь Buffer владеет сырым массивом, поэтому нужны все пять специальных
// функций-членов. Дополнительно используется copy-and-swap для строгой
// гарантии исключений в operator=.
class Buffer {
public:
    explicit Buffer(std::size_t n = 0)
        : size_(n), data_(n ? new int[n]{} : nullptr) {}

    // 1. Деструктор
    ~Buffer() { delete[] data_; }

    // 2. Копирующий конструктор — глубокая копия
    Buffer(const Buffer& other)
        : size_(other.size_), data_(other.size_ ? new int[other.size_]{} : nullptr) {
        for (std::size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
    }

    // 3. Перемещающий конструктор — «кража» ресурса
    Buffer(Buffer&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
    }

    // 4. Копирующее присваивание — через copy-and-swap
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            Buffer tmp(other);      // может бросить — исходный объект не тронут
            swap(tmp);              // noexcept
        }
        return *this;
    }

    // 5. Перемещающее присваивание
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    void swap(Buffer& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
    }

    std::size_t size() const { return size_; }
    int* data() { return data_; }
    const int* data() const { return data_; }

    int& operator[](std::size_t i) { return data_[i]; }
    int operator[](std::size_t i) const { return data_[i]; }

private:
    std::size_t size_;
    int* data_;
};

inline void swap(Buffer& a, Buffer& b) noexcept { a.swap(b); }