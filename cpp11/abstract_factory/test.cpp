#include <gtest/gtest.h>
#include "abstract_factory.hpp"

TEST(AbstractFactoryTest, CreatesFamily) {
    WinFactory f;
    auto b = f.createButton();
    auto c = f.createCheckbox();
    EXPECT_NE(b, nullptr);
    EXPECT_NE(c, nullptr);
}

TEST(AbstractFactoryTest, MacFamily) {
    MacFactory f;
    auto b = f.createButton();
    auto c = f.createCheckbox();
    EXPECT_NE(b, nullptr);
    EXPECT_NE(c, nullptr);
}