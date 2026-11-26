#include <gtest/gtest.h>
#include "atomic_ref.hpp"
#include <thread>
#include <vector>

TEST(AtomicRefTest, StatsSingleThread) {
    Stats s;
    recordHit(s);
    recordHit(s);
    recordMiss(s);
    EXPECT_EQ(s.hits, 2);
    EXPECT_EQ(s.misses, 1);
}

TEST(AtomicRefTest, StatsManyThreads) {
    Stats s;
    std::vector<std::thread> threads;
    for (int i = 0; i < 8; ++i) {
        threads.emplace_back([&] {
            for (int j = 0; j < 1000; ++j) recordHit(s);
        });
    }
    for (auto& t : threads) t.join();
    EXPECT_EQ(s.hits, 8000);
}

TEST(AtomicRefTest, WorkerDoneFlag) {
    Worker w;
    EXPECT_FALSE(isDone(w));
    std::thread t([&] { runWorker(w); });
    t.join();
    EXPECT_TRUE(isDone(w));
    EXPECT_EQ(w.result, 42);
}

TEST(AtomicRefTest, CounterLimit) {
    Counter c;
    EXPECT_TRUE(tryIncrementIfLess(c, 3));
    EXPECT_TRUE(tryIncrementIfLess(c, 3));
    EXPECT_TRUE(tryIncrementIfLess(c, 3));
    EXPECT_FALSE(tryIncrementIfLess(c, 3));
    EXPECT_EQ(c.value, 3);
}

TEST(AtomicRefTest, CounterManyThreads) {
    Counter c;
    std::vector<std::thread> threads;
    for (int i = 0; i < 8; ++i) {
        threads.emplace_back([&] {
            for (int j = 0; j < 1000; ++j) {
                tryIncrementIfLess(c, 5000);
            }
        });
    }
    for (auto& t : threads) t.join();
    EXPECT_EQ(c.value, 5000);   // ровно лимит, не больше
}