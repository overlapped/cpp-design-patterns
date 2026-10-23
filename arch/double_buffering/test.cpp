#include <gtest/gtest.h>
#include "double_buffering.hpp"

TEST(DoubleBufferTest, InitialIndices) {
    DoubleBuffer<int, 4> db;
    EXPECT_EQ(db.front_index(), 0u);
    EXPECT_EQ(db.back_index(), 1u);
}

TEST(DoubleBufferTest, SwapChangesActiveBuffers) {
    DoubleBuffer<int, 3> db;
    db.back() = {1, 2, 3};
    db.swap();
    EXPECT_EQ(db.front()[0], 1);
    EXPECT_EQ(db.front()[2], 3);
    EXPECT_EQ(db.front_index(), 1u);
    EXPECT_EQ(db.back_index(), 0u);
}

TEST(DoubleBufferTest, FrontStableUntilSwap) {
    DoubleBuffer<int, 2> db;
    db.back() = {7, 8};
    db.swap();
    EXPECT_EQ(db.front()[0], 7);

    // Пишем новый кадр, но не свопаем — front не должен меняться
    db.back() = {9, 9};
    EXPECT_EQ(db.front()[0], 7);
    EXPECT_EQ(db.front()[1], 8);
}

TEST(DoubleBufferTest, TwoSwapsRestoreOrder) {
    DoubleBuffer<int, 1> db;
    db.back() = {1};
    db.swap();
    db.back() = {2};
    db.swap();
    EXPECT_EQ(db.front()[0], 2);
    EXPECT_EQ(db.back()[0], 1);   // старый кадр остался в back
}

TEST(DoubleBufferTest, SizeIsN) {
    static_assert(DoubleBuffer<int, 8>::size() == 8);
    SUCCEED();
}