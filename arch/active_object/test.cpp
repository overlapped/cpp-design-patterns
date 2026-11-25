#include <gtest/gtest.h>
#include "active_object.hpp"
#include <thread>
#include <vector>

TEST(ActiveObjectTest, SequentialIncrements) {
    ActiveCounter c;
    auto f1 = c.increment();
    auto f2 = c.increment();
    f1.get();
    f2.get();
    EXPECT_EQ(c.get().get(), 2);
}

TEST(ActiveObjectTest, ConcurrentIncrementsFromManyThreads) {
    ActiveCounter c;
    constexpr int kThreads = 8;
    constexpr int kPerThread = 100;

    std::vector<std::thread> threads;
    threads.reserve(kThreads);
    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&c] {
            for (int i = 0; i < kPerThread; ++i) {
                c.increment().get();   // ждём, чтобы гарантировать порядок
            }
        });
    }
    for (auto& th : threads) th.join();

    EXPECT_EQ(c.get().get(), kThreads * kPerThread);
}

TEST(ActiveObjectTest, DestructorDrainsQueue) {
    // Если деструктор не дождётся задач, тест под ASan поймает use-after-free
    {
        ActiveCounter c;
        for (int i = 0; i < 5; ++i) (void)c.increment();
        // c уничтожается здесь — все задачи должны выполниться или отмениться
    }
    SUCCEED();
}

TEST(ActiveObjectTest, GetIsSerializedWithIncrement) {
    ActiveCounter c;
    auto inc = c.increment();
    auto get = c.get();
    inc.get();
    // get поставлен в очередь после inc, значит увидит результат
    EXPECT_EQ(get.get(), 1);
}