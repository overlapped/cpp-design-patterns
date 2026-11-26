#include "attorney_client_passkey.hpp"

#include <iostream>

int main() {
    // 1. Демонстрация Attorney-Client
    idioms::BankAccount account("Alice", 1000.0);
    account.deposit(250.0);
    account.deposit(50.0);

    // Обычный код не видит internal_transaction_count(), но Auditor получает его через Attorney
    std::cout << "[Attorney-Client] Audit report -> "
              << idioms::Auditor::generate_report(account) << '\n';

    // 2. Демонстрация Passkey
    idioms::DatabaseConnection conn(42);
    conn.execute_query();
    conn.execute_query();
    std::cout << "[Passkey] Queries before recycle: " << conn.query_count() << '\n';

    // conn.reset_state({}); // Ошибка компиляции: конструктор Passkey приватен!
    idioms::ConnectionPool::recycle(conn);
    std::cout << "[Passkey] Queries after recycle:  " << conn.query_count() << '\n';

    return 0;
}
