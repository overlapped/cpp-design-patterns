#include <gtest/gtest.h>
#include "dry.hpp"

TEST(DryTest, SharedLogicMatchesDuplicated) {
    EXPECT_EQ(good::formatUser("Alice", 30), bad::formatUser("Alice", 30));
    EXPECT_EQ(good::formatAdmin("Bob", 40), bad::formatAdmin("Bob", 40));
}

#if __cplusplus >= 202002L
TEST(DryTest, ConceptBased) {
    struct P { std::string name; int age; };
    EXPECT_EQ(modern::format(P{"Eve", 22}), good::formatUser("Eve", 22));
}
#endif