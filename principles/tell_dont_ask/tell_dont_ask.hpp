#pragma once
#include <string>
#include <stdexcept>

// Tell, Don't Ask: не спрашивай у объекта данные, чтобы принять
// решение снаружи, — скажи объекту, что делать. Логика должна
// жить там, где живут данные.
//
// «Плохо»: клиент дёргает геттеры и сам решает, можно ли снять
// деньги. «Хорошо»: клиент говорит «сними 100», а объект сам
// проверяет инвариант.

// ❌ Плохо: клиент спрашивает баланс и сам решает
namespace bad {
    class Account {
    public:
        explicit Account(int balance) : balance_(balance) {}

        int balance() const { return balance_; }
        void setBalance(int b) { balance_ = b; }

    private:
        int balance_;
    };

    // Логика снятия — снаружи, в клиенте
    inline void withdraw(Account& acc, int amount) {
        if (acc.balance() < amount) {
            throw std::runtime_error("insufficient funds");
        }
        acc.setBalance(acc.balance() - amount);
    }
}

// ✅ Хорошо: объект сам знает, можно ли снять деньги
namespace good {
    class Account {
    public:
        explicit Account(int balance) : balance_(balance) {}

        // Command: объект сам проверяет инвариант
        void withdraw(int amount) {
            if (amount <= 0) throw std::invalid_argument("amount must be > 0");
            if (amount > balance_) throw std::runtime_error("insufficient funds");
            balance_ -= amount;
        }

        void deposit(int amount) {
            if (amount <= 0) throw std::invalid_argument("amount must be > 0");
            balance_ += amount;
        }

        // Query: только для чтения, без принятия решений
        int balance() const { return balance_; }

    private:
        int balance_;
    };
}

// Второй пример: «плохо» — клиент проверяет статус и решает,
// можно ли отправить сообщение. «Хорошо» — объект сам решает.
namespace bad2 {
    class User {
    public:
        explicit User(bool active) : active_(active) {}
        bool isActive() const { return active_; }
        void sendEmail(const std::string&) { /* ... */ }
    private:
        bool active_;
    };
}

namespace good2 {
    class User {
    public:
        explicit User(bool active) : active_(active) {}

        // Объект сам решает, можно ли отправить письмо
        void sendEmail(const std::string& msg) {
            if (!active_) throw std::runtime_error("user is inactive");
            // ... отправка
            (void)msg;
        }

    private:
        bool active_;
    };
}