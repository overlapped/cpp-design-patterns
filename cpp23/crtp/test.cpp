#include <gtest/gtest.h>
#include "crtp.hpp"

TEST(DeducingThisTest, BaseCallsDerivedMethod) {
    Name n{"Bob"};
    // Если to_string() недоступен — компилятор ошибётся здесь
    EXPECT_EQ(n.describe(), "[Bob]");
}

TEST(DeducingThisTest, DifferentDerivedTypes) {
    Point p{1, 2};
    EXPECT_EQ(p.describe(), "[(1, 2)]");
}

TEST(DeducingThisTest, NoTemplateParameter) {
    // Просто проверка, что Name наследует Printable (не шаблон)
    static_assert(std::is_base_of_v<Printable, Name>);
    SUCCEED();
}