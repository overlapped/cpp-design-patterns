#include <gtest/gtest.h>
#include "memory_pool.hpp"

struct Obj {
    int value;
    explicit Obj(int v) : value(v) {}
};

TEST(MemoryPoolTest, InitiallyFull) {
    MemoryPool<Obj, 8> pool;
    EXPECT_EQ(pool.available(), 8u);
    EXPECT_EQ(pool.capacity(), 8u);
}

TEST(MemoryPoolTest, CreateAndDestroy) {
    MemoryPool<Obj, 2> pool;
    Obj* a = pool.create(1);
    Obj* b = pool.create(2);
    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);
    EXPECT_EQ(a->value, 1);
    EXPECT_EQ(b->value, 2);
    EXPECT_EQ(pool.available(), 0u);

    pool.destroy(a);
    pool.destroy(b);
    EXPECT_EQ(pool.available(), 2u);
}

TEST(MemoryPoolTest, ExhaustionReturnsNull) {
    MemoryPool<Obj, 1> pool;
    Obj* a = pool.create(1);
    ASSERT_NE(a, nullptr);
    EXPECT_EQ(pool.create(2), nullptr);
    pool.destroy(a);
    EXPECT_NE(pool.create(3), nullptr);
}

TEST(MemoryPoolTest, ReusesFreedBlock) {
    MemoryPool<Obj, 1> pool;
    Obj* a = pool.create(1);
    pool.destroy(a);
    Obj* b = pool.create(2);
    EXPECT_EQ(a, b);   // тот же адрес
    EXPECT_EQ(b->value, 2);
}

TEST(MemoryPoolTest, DestructorCalledOnDestroy) {
    static int destroyed = 0;
    struct Tracked {
        int v;
        explicit Tracked(int x) : v(x) {}
        ~Tracked() { ++destroyed; }
    };
    destroyed = 0;

    MemoryPool<Tracked, 1> pool;
    Tracked* t = pool.create(42);
    ASSERT_NE(t, nullptr);
    pool.destroy(t);
    EXPECT_EQ(destroyed, 1);
}

TEST(MemoryPoolTest, RespectsAlignment) {
    struct alignas(64) Aligned { int v; };
    MemoryPool<Aligned, 2> pool;
    Aligned* a = pool.create(1);
    ASSERT_NE(a, nullptr);
    auto addr = reinterpret_cast<std::uintptr_t>(a);
    EXPECT_EQ(addr % 64, 0u);
}