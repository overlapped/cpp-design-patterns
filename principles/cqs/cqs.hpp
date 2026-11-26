#pragma once
#include <string>
#include <vector>
#include <optional>
#include <stdexcept>
#include <cstddef>

// CQS (Command-Query Separation): методы делятся на две категории.
// - Query: возвращает данные, НЕ меняет состояние (const).
// - Command: меняет состояние, НЕ возвращает данные (void).
//
// Смешивание — источник неожиданных побочных эффектов:
// «getSize()» не должен удалять элементы, «pop()» не должен
// возвращать значение И менять размер (иначе нельзя вызвать
// pop() без получения значения, и нельзя проверить, что pop()
// безопасен).

// ❌ Плохо: query с побочным эффектом и command с возвратом
namespace bad {
    class Stack {
    public:
        void push(int v) { data_.push_back(v); }

        // Query, который меняет состояние — нарушение CQS
        int getSize() {
            int n = static_cast<int>(data_.size());
            data_.clear();     // ← побочный эффект в query
            return n;
        }

        // Command, который возвращает значение — нарушение CQS
        int pop() {
            int v = data_.back();
            data_.pop_back();
            return v;
        }

    private:
        std::vector<int> data_;
    };
}

// ✅ Хорошо: query — const, command — void
namespace good {
    class Stack {
    public:
        // Command: меняет состояние, возвращает void
        void push(int v) { data_.push_back(v); }

        // Command: меняет состояние, возвращает void
        // Проверка «не пуст ли» — отдельный query
        void pop() {
            if (data_.empty()) throw std::runtime_error("pop on empty");
            data_.pop_back();
        }

        // Query: const, не меняет состояние
        std::optional<int> top() const {
            if (data_.empty()) return std::nullopt;
            return data_.back();
        }

        // Query: const, не меняет состояние
        bool empty() const { return data_.empty(); }

        // Query: const, не меняет состояние
        std::size_t size() const { return data_.size(); }

    private:
        std::vector<int> data_;
    };
}