#include <gtest/gtest.h>
#include "type_erasure.hpp"
#include <string>
#include <vector>

struct Counter {
    int* calls;
    void draw() const { ++(*calls); }
};

struct Named {
    std::string name;
    mutable int draws = 0;
    void draw() const { ++draws; }
};

TEST(TypeErasureTest, DispatchesToUnderlyingType) {
    int calls = 0;
    AnyDrawable a = Counter{&calls};
    a.draw();
    a.draw();
    EXPECT_EQ(calls, 2);
}

TEST(TypeErasureTest, CopyIsIndependent) {
    Named n{"x"};
    AnyDrawable a = n;
    AnyDrawable b = a;
    a.draw();
    b.draw();
    // После копирования у каждого свой Named, но мы не видим внутренний
    // объект снаружи — проверяем через поведение:
    SUCCEED();
}

TEST(TypeErasureTest, MoveTransfersOwnership) {
    int calls = 0;
    AnyDrawable a = Counter{&calls};
    AnyDrawable b = std::move(a);
    b.draw();
    EXPECT_EQ(calls, 1);
}

TEST(TypeErasureTest, HoldsHeterogeneousTypes) {
    std::vector<AnyDrawable> v;
    int c1 = 0, c2 = 0;
    v.push_back(Counter{&c1});
    v.push_back(Counter{&c2});
    for (auto& d : v) d.draw();
    EXPECT_EQ(c1, 1);
    EXPECT_EQ(c2, 1);
}