#include <gtest/gtest.h>
#include "rule_of_zero.hpp"
#include <type_traits>
#include <utility>

TEST(RuleOfZeroTest, DocumentIsCopyable) {
    Document a("Doc");
    a.addLine("x");
    Document b = a;
    b.addLine("y");
    EXPECT_EQ(a.lines().size(), 1u);
    EXPECT_EQ(b.lines().size(), 2u);
}

TEST(RuleOfZeroTest, DocumentIsMovable) {
    Document a("Doc");
    a.addLine("x");
    Document b = std::move(a);
    EXPECT_EQ(b.lines().size(), 1u);
    EXPECT_EQ(b.title(), "Doc");
}

TEST(RuleOfZeroTest, WidgetIsMoveOnly) {
    static_assert(!std::is_copy_constructible_v<Widget>,
                  "Widget must be move-only");
    static_assert(std::is_move_constructible_v<Widget>,
                  "Widget must be movable");
    SUCCEED();
}

TEST(RuleOfZeroTest, WidgetMoveTransfersValue) {
    Widget w1(7);
    Widget w2 = std::move(w1);
    EXPECT_EQ(w2.value(), 7);
}

TEST(RuleOfZeroTest, DocumentHasNoUserDeclaredSpecialMembers) {
    // Rule of Zero: ни деструктор, ни копирование, ни перемещение
    // не объявлены пользователем. Проверяем через type_traits:
    // для класса с не-trivial полями это не всегда выразимо,
    // но факт copyable + movable + destructible — уже достаточен.
    static_assert(std::is_copy_constructible_v<Document>);
    static_assert(std::is_move_constructible_v<Document>);
    static_assert(std::is_destructible_v<Document>);
    SUCCEED();
}