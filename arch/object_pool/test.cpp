#include <gtest/gtest.h>
#include "object_pool.hpp"
#include <memory>

struct Dummy {
    int value;
    explicit Dummy(int v) : value(v) {}
};

TEST(ObjectPoolTest, InitiallyFull) {
    int next = 0;
    ObjectPool<Dummy> pool(3, [&] { return std::make_unique<Dummy>(next++); });
    EXPECT_EQ(pool.available(), 3u);
}

TEST(ObjectPoolTest, AcquireReducesAvailable) {
    int next = 0;
    ObjectPool<Dummy> pool(2, [&] { return std::make_unique<Dummy>(next++); });
    auto a = pool.acquire();
    EXPECT_EQ(pool.available(), 1u);
    auto b = pool.acquire();
    EXPECT_EQ(pool.available(), 0u);
}

TEST(ObjectPoolTest, AcquireFromEmptyCreatesNew) {
    int next = 0;
    ObjectPool<Dummy> pool(1, [&] { return std::make_unique<Dummy>(next++); });
    auto a = pool.acquire();   // из пула
    auto b = pool.acquire();   // пул пуст — создан новый
    EXPECT_NE(a, nullptr);
    EXPECT_NE(b, nullptr);
    EXPECT_EQ(pool.available(), 0u);
}

TEST(ObjectPoolTest, ReleaseReturnsToPool) {
    int next = 0;
    ObjectPool<Dummy> pool(2, [&] { return std::make_unique<Dummy>(next++); });
    auto a = pool.acquire();
    EXPECT_EQ(pool.available(), 1u);
    pool.release(std::move(a));
    EXPECT_EQ(pool.available(), 2u);
}

TEST(ObjectPoolTest, ReusesReleasedObject) {
    int next = 0;
    ObjectPool<Dummy> pool(1, [&] { return std::make_unique<Dummy>(next++); });
    auto a = pool.acquire();
    int firstValue = a->value;
    pool.release(std::move(a));

    auto b = pool.acquire();
    // Тот же объект, что вернули, — значение сохранилось
    EXPECT_EQ(b->value, firstValue);
    EXPECT_EQ(next, 1);   // новый объект не создавался
}

TEST(ObjectPoolTest, ReleaseNullIsSafe) {
    int next = 0;
    ObjectPool<Dummy> pool(1, [&] { return std::make_unique<Dummy>(next++); });
    EXPECT_NO_THROW(pool.release(nullptr));
    EXPECT_EQ(pool.available(), 1u);
}