#include <gtest/gtest.h>
#include "factory_method.hpp"

TEST(FactoryMethodTest, ViaEnum) {
    auto pc = makeCreator(DocType::Pdf);
    auto wc = makeCreator(DocType::Word);
    EXPECT_EQ(pc->create()->type(), "PDF");
    EXPECT_EQ(wc->create()->type(), "Word");
}

TEST(FactoryMethodTest, DirectCreator) {
    PdfCreator c;
    EXPECT_EQ(c.create()->type(), "PDF");
}