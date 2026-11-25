#include <gtest/gtest.h>
#include "lsp.hpp"

TEST(LspTest, BadSquareBreaksRectangleContract) {
    bad::Rectangle& r = *new bad::Square();
    r.setWidth(3); r.setHeight(4);
    EXPECT_NE(r.area(), 12.0);   // контракт Rectangle нарушен
    delete &r;
}

TEST(LspTest, GoodTypesRespectContract) {
    good::Rectangle gr(3, 4);
    EXPECT_DOUBLE_EQ(gr.area(), 12.0);
    good::Square gs(3);
    EXPECT_DOUBLE_EQ(gs.area(), 9.0);
}