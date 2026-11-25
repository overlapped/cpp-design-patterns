#pragma once
#include <string>
#include <memory>

// ❌ Плохо: цепочка вызовов через несколько объектов
// Клиент знает о Wallet, Wallet знает о Currency — клиенту это не нужно
namespace bad {
    struct Currency {
        std::string code;
        double amount = 0;
    };

    class Wallet {
    public:
        Currency& currency() { return currency_; }
    private:
        Currency currency_{"USD", 100.0};
    };

    class Person {
    public:
        Wallet& wallet() { return wallet_; }
    private:
        Wallet wallet_;
    };

    inline double getAmount(Person& p) {
        // Нарушение LoD: Person -> Wallet -> Currency
        return p.wallet().currency().amount;
    }
}

// ✅ Хорошо: Person сам отвечает на вопрос о деньгах
namespace good {
    struct Currency {
        std::string code;
        double amount = 0;
    };

    class Wallet {
    public:
        double amount() const { return currency_.amount; }
        const std::string& code() const { return currency_.code; }
    private:
        Currency currency_{"USD", 100.0};
    };

    class Person {
    public:
        double money() const { return wallet_.amount(); }
        const std::string& currencyCode() const { return wallet_.code(); }
    private:
        Wallet wallet_;
    };

    inline double getAmount(const Person& p) {
        // Клиент общается только с Person — один «друг»
        return p.money();
    }
}