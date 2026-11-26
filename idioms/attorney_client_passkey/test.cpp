#include "attorney_client_passkey.hpp"

#include <gtest/gtest.h>
#include <type_traits>

TEST(AttorneyClientTest, AuditorCanReadRestrictedMetadata) {
    idioms::BankAccount acc("Bob", 500.0);
    acc.deposit(100.0);
    acc.deposit(200.0);

    EXPECT_DOUBLE_EQ(acc.balance(), 800.0);
    EXPECT_EQ(idioms::Auditor::generate_report(acc), "Owner: Bob, Transactions: 2");
}

TEST(PasskeyTest, OnlyAuthorizedClassCanInvokeKeyedMethod) {
    idioms::DatabaseConnection conn(1);
    conn.execute_query();
    conn.execute_query();
    ASSERT_EQ(conn.query_count(), 2);

    // Проверяем на этапе компиляции, что посторонний код не может создать Passkey
    static_assert(!std::is_default_constructible_v<idioms::Passkey<idioms::ConnectionPool>>,
                  "Passkey must not be default-constructible by outsiders");
    static_assert(!std::is_copy_constructible_v<idioms::Passkey<idioms::ConnectionPool>>,
                  "Passkey must not be copyable");

    // Авторизованный пул соединений успешно сбрасывает состояние
    idioms::ConnectionPool::recycle(conn);
    EXPECT_EQ(conn.query_count(), 0);
    EXPECT_TRUE(conn.is_open());
}
