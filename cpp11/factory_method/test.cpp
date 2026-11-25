#include <gtest/gtest.h>
#include "factory_method.hpp"

TEST(FactoryMethodTest, CreatesPdf) {
    PdfCreator c;
    EXPECT_EQ(c.create()->type(), "PDF");
}

TEST(FactoryMethodTest, CreatesWord) {
    WordCreator c;
    EXPECT_EQ(c.create()->type(), "Word");
}