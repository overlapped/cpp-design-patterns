#include <gtest/gtest.h>
#include "flyweight.hpp"

TEST(FlyweightTest, SharesInstances) {
    GlyphFactory f;
    EXPECT_EQ(f.get('x'), f.get('x'));
    EXPECT_NE(f.get('x'), f.get('y'));
    EXPECT_EQ(f.size(), 2);
}