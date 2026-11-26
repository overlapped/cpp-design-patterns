#pragma once
#include <memory>
#include <string>
#include <vector>

// Rule of Zero: если класс не управляет ресурсами напрямую
// (использует умные указатели, контейнеры, std::string), то НЕ нужно
// писать деструктор, копирование и перемещение — компилятор сгенерирует
// их корректно.
//
// Контраст с Rule of Five: там сырой int* и пять специальных членов.
// Здесь — ни одного специального члена, но поведение корректное.

class Document {
public:
    explicit Document(std::string title)
        : title_(std::move(title)) {}

    void addLine(std::string line) {
        lines_.push_back(std::move(line));
    }

    const std::string& title() const { return title_; }
    const std::vector<std::string>& lines() const { return lines_; }

    // Ни деструктора, ни копирования, ни перемещения — Rule of Zero.
    // std::string и std::vector уже реализуют Rule of Five корректно.

private:
    std::string title_;
    std::vector<std::string> lines_;
};

// Второй пример: владение через std::unique_ptr.
// Класс move-only, но специальные члены не написаны вручную.
class Widget {
public:
    explicit Widget(int value) : impl_(std::make_unique<Impl>(value)) {}

    int value() const { return impl_->value; }

    // unique_ptr делает Widget move-only автоматически:
    // копирование удалено, перемещение сгенерировано.
    // Деструктор тоже сгенерирован — unique_ptr освободит Impl.

private:
    struct Impl {
        explicit Impl(int v) : value(v) {}
        int value;
    };
    std::unique_ptr<Impl> impl_;
};