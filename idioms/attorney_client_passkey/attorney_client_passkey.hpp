#pragma once

#include <string>
#include <utility>

namespace idioms {

// ============================================================================
// 1. Идиома Attorney-Client (Адвокат — Клиент)
// ============================================================================
// Проблема: если объявить `friend class Auditor;`, Auditor получит доступ ко ВСЕМ
// приватным полям и методам класса BankAccount (включая изменение баланса).
// Решение: класс BankAccount объявляет другом только класс-посредник (Attorney),
// который открывает ровно те методы, которые нужны конкретному клиенту (Auditor).

class AccountAttorney;

class BankAccount {
public:
    BankAccount(std::string owner, double initial_balance)
        : owner_(std::move(owner)), balance_(initial_balance) {}

    void deposit(double amount) {
        if (amount > 0.0) {
            balance_ += amount;
            ++transaction_count_;
        }
    }

    [[nodiscard]] double balance() const noexcept { return balance_; }

private:
    // Секретный метод, который НЕ должен быть доступен даже аудитору
    void override_balance_for_admin(double new_balance) noexcept {
        balance_ = new_balance;
    }

    // Внутренний метод, который нужно открыть только для аудитора
    [[nodiscard]] int internal_transaction_count() const noexcept {
        return transaction_count_;
    }

    [[nodiscard]] const std::string& internal_owner() const noexcept {
        return owner_;
    }

    // Единственный друг — «Адвокат»
    friend class AccountAttorney;

    std::string owner_;
    double balance_{0.0};
    int transaction_count_{0};
};

// «Адвокат» (Attorney): не имеет состояния, все методы статические и приватные.
// Друзьями объявляются конкретные классы-клиенты, которым нужен ограниченный доступ.
class AccountAttorney {
private:
    static int get_transaction_count(const BankAccount& acc) noexcept {
        return acc.internal_transaction_count();
    }

    static const std::string& get_owner(const BankAccount& acc) noexcept {
        return acc.internal_owner();
    }

    // Открываем доступ к этим двум методам только для Auditor
    friend class Auditor;
};

class Auditor {
public:
    [[nodiscard]] static std::string generate_report(const BankAccount& acc) {
        // Auditor может читать владельца и число транзакций через адвоката,
        // но НЕ имеет доступа к override_balance_for_admin() или приватным полям.
        return "Owner: " + AccountAttorney::get_owner(acc) +
               ", Transactions: " + std::to_string(AccountAttorney::get_transaction_count(acc));
    }
};

// ============================================================================
// 2. Идиома Passkey (Ключ доступа)
// ============================================================================
// Более лаконичная альтернатива Attorney-Client без промежуточного класса-прокси.
// Целевой метод остаётся PUBLIC, но принимает специальный пустой объект-ключ (Passkey),
// сконструировать который могут только избранные классы-друзья.

template <typename T>
class Passkey {
private:
    friend T;
    Passkey() = default;

    // Запрещаем копирование/перемещение, чтобы ключ нельзя было передать третьим лицам
    Passkey(const Passkey&) = delete;
    Passkey& operator=(const Passkey&) = delete;
};

class ConnectionPool; // Вперёд объявляем класс, которому выдадим ключ

class DatabaseConnection {
public:
    explicit DatabaseConnection(int id) : id_(id) {}

    [[nodiscard]] int id() const noexcept { return id_; }
    [[nodiscard]] bool is_open() const noexcept { return is_open_; }

    // Метод формально public, но вызвать его можно только при наличии Passkey<ConnectionPool>
    void reset_state(Passkey<ConnectionPool>) noexcept {
        is_open_ = true;
        query_count_ = 0;
    }

    void execute_query() noexcept {
        if (is_open_) {
            ++query_count_;
        }
    }

    [[nodiscard]] int query_count() const noexcept { return query_count_; }

private:
    int id_;
    bool is_open_{true};
    int query_count_{0};
};

class ConnectionPool {
public:
    static void recycle(DatabaseConnection& conn) noexcept {
        // Только ConnectionPool может создать временный объект Passkey<ConnectionPool>{}
        conn.reset_state(Passkey<ConnectionPool>{});
    }
};

} // namespace idioms
