#include "thread_pool.hpp"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <numeric>
#include <stdexcept>
#include <thread>
#include <vector>

TEST(ThreadPoolTest, ExecutesTasksAndReturnsResults) {
    arch::ThreadPool pool(4);

    auto f1 = pool.submit([](int a, int b) { return a + b; }, 10, 25);
    auto f2 = pool.submit([]() { return std::string("hello"); });

    EXPECT_EQ(f1.get(), 35);
    EXPECT_EQ(f2.get(), "hello");
}

TEST(ThreadPoolTest, ExecutesManyTasksConcurrently) {
    constexpr std::size_t kTaskCount = 100;
    arch::ThreadPool pool(4);

    std::atomic<int> counter{0};
    std::vector<std::future<int>> futures;
    futures.reserve(kTaskCount);

    for (std::size_t i = 0; i < kTaskCount; ++i) {
        futures.push_back(pool.submit([&counter, i]() {
            counter.fetch_add(1, std::memory_order_relaxed);
            return static_cast<int>(i);
        }));
    }

    int sum = 0;
    for (auto& f : futures) {
        sum += f.get();
    }

    EXPECT_EQ(counter.load(std::memory_order_relaxed), static_cast<int>(kTaskCount));
    EXPECT_EQ(sum, static_cast<int>((kTaskCount - 1) * kTaskCount / 2));
}

TEST(ThreadPoolTest, PropagatesExceptionsThroughFuture) {
    arch::ThreadPool pool(2);

    auto future = pool.submit([]() -> int {
        throw std::logic_error("Task failed");
    });

    EXPECT_THROW(future.get(), std::logic_error);
}

TEST(ThreadPoolTest, HandlesZeroThreadCountGracefully) {
    arch::ThreadPool pool(0);
    EXPECT_EQ(pool.worker_count(), 1u);

    auto future = pool.submit([]() { return 42; });
    EXPECT_EQ(future.get(), 42);
}

TEST(ThreadPoolTest, CompletesPendingTasksOnDestruction) {
    constexpr int kTasks = 20;
    std::atomic<int> completed{0};

    {
        arch::ThreadPool pool(2);
        for (int i = 0; i < kTasks; ++i) {
            pool.submit([&completed]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                completed.fetch_add(1, std::memory_order_relaxed);
            });
        }
        // Деструктор должен дождаться завершения всех задач из очереди
    }

    EXPECT_EQ(completed.load(std::memory_order_relaxed), kTasks);
}
