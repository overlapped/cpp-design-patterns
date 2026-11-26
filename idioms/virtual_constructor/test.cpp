#include <gtest/gtest.h>
#include "virtual_constructor.hpp"
#include <memory>

TEST(VirtualConstructorTest, CloneReturnsCorrectType) {
    Circle c(1.0);
    auto cloned = c.clone();
    EXPECT_EQ(cloned->name(), "Circle");
    EXPECT_DOUBLE_EQ(cloned->area(), c.area());
}

TEST(VirtualConstructorTest, CloneThroughBasePointer) {
    std::unique_ptr<Shape> base = std::make_unique<Rectangle>(2.0, 3.0);
    auto cloned = base->clone();
    EXPECT_EQ(cloned->name(), "Rectangle");
    EXPECT_DOUBLE_EQ(cloned->area(), 6.0);
}

TEST(VirtualConstructorTest, CloneIsIndependent) {
    auto original = std::make_unique<Circle>(5.0);
    auto copy = original->clone();
    original.reset();                 // уничтожаем оригинал
    EXPECT_EQ(copy->name(), "Circle"); // копия жива
    EXPECT_DOUBLE_EQ(copy->area(), 3.14159 * 25.0);
}

TEST(VirtualConstructorTest, CloneAllPreservesOrder) {
    std::vector<std::unique_ptr<Shape>> v;
    v.push_back(std::make_unique<Circle>(1.0));
    v.push_back(std::make_unique<Rectangle>(2.0, 3.0));
    v.push_back(std::make_unique<Circle>(4.0));

    auto copy = cloneAll(v);
    ASSERT_EQ(copy.size(), 3u);
    EXPECT_EQ(copy[0]->name(), "Circle");
    EXPECT_EQ(copy[1]->name(), "Rectangle");
    EXPECT_EQ(copy[2]->name(), "Circle");
}

TEST(VirtualConstructorTest, CloneAllEmpty) {
    std::vector<std::unique_ptr<Shape>> empty;
    auto copy = cloneAll(empty);
    EXPECT_TRUE(copy.empty());
}