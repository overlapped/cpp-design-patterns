#pragma once
#include <stdexcept>
#include <string>
#include <vector>
#include <cstddef>

// ❌ Плохо: ошибка «прячется», невалидное состояние живёт дальше
namespace bad {
    class BankAccount {
    public:
        void deposit(double amount) {
            if (amount <= 0) return;   // молча игнорируем — ошибка потеряна
            balance_ += amount;
        }
        void withdraw(double amount) {
            balance_ -= amount;        // уходим в минус без предупреждения
        }
        double balance() const { return balance_; }
    private:
        double balance_ = 0;
    };
}

// ✅ Хорошо: невалидный вход → исключение сразу, инвариант держится
namespace good {
    class BankAccount {
    public:
        void deposit(double amount) {
            if (amount <= 0)
                throw std::invalid_argument("deposit: amount must be > 0");
            balance_ += amount;
        }
        void withdraw(double amount) {
            if (amount <= 0)
                throw std::invalid_argument("withdraw: amount must be > 0");
            if (amount > balance_)
                throw std::runtime_error("withdraw: insufficient funds");
            balance_ -= amount;
        }
        double balance() const { return balance_; }
    private:
        double balance_ = 0;
    };

    // Fail fast в конструкторе: объект не может существовать в невалидном виде
    class Range {
    public:
        Range(int lo, int hi) : lo_(lo), hi_(hi) {
            if (lo > hi)
                throw std::invalid_argument("Range: lo must be <= hi");
        }
        bool contains(int x) const { return x >= lo_ && x <= hi_; }
    private:
        int lo_, hi_;
    };
}