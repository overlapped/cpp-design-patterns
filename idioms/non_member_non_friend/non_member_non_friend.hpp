#pragma once
#include <ostream>
#include <string>
#include <utility>

// Класс Money с минимальной поверхностью: только конструктор и два
// публичных read-only метода. Всё остальное — свободные функции ниже.
class Money {
public:
    Money() = default;
    Money(long long amount, std::string currency)
        : amount_(amount), currency_(std::move(currency)) {}

    long long amount() const { return amount_; }
    const std::string& currency() const { return currency_; }

    // Мутирующие операции — методами, потому что они меняют состояние.
    // (В реальном коде их можно сделать членами или тоже свободными,
    //  если есть публичный setter. Здесь оставим членами.)
    Money& operator+=(const Money& other) {
        amount_ += other.amount_;
        return *this;
    }

private:
    long long amount_ = 0;
    std::string currency_;
};

// --- Non-member, non-friend interface ---
// Эти функции не объявлены friend, поэтому не имеют доступа к приватным
// полям. Они работают только через публичные amount()/currency().
// Это позволяет менять внутреннее представление Money, не трогая их.

inline Money operator+(Money lhs, const Money& rhs) {
    lhs += rhs;
    return lhs;
}

inline bool operator==(const Money& a, const Money& b) {
    return a.amount() == b.amount() && a.currency() == b.currency();
}

inline bool operator!=(const Money& a, const Money& b) {
    return !(a == b);
}

inline std::ostream& operator<<(std::ostream& os, const Money& m) {
    os << m.amount() << ' ' << m.currency();
    return os;
}