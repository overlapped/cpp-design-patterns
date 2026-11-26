#include "read_write_lock.hpp"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

TEST(ReadWriteLockTest, BasicCrudOperations) {
    arch::ThreadSafeCache<std::string, int> cache;

    EXPECT_FALSE(cache.contains("a"));
    EXPECT_EQ(cache.get("a"), std::nullopt);

    cache.set("a", 42);
    EXPECT_TRUE(cache.contains("a"));
    EXPECT_EQ(cache.get("a"), 42);
    EXPECT_EQ(cache.size(), 1u);

    cache.set("a", 99);
    EXPECT_EQ(cache.get("a"), 99);

    EXPECT_TRUE(cache.erase("a"));
    EXPECT_FALSE(cache.erase("a"));
    EXPECT_EQ(cache.size(), 0u);
}

TEST(ReadWriteLockTest, AllowsConcurrentReaders) {
    arch::ThreadSafeCache<int, int> cache;
    cache.set(1, 100);

    std::atomic<int> active_readers{0};
    std::atomic<int> max_simultaneous_readers{0};

    constexpr int kNumReaders = 4;
    std::vector<std::thread> readers;

    for (int i = 0; i < kNumReaders; ++i) {
        readers.emplace_back([&]() {
            cache.read_with(1, [&](const int& val) {
                EXPECT_EQ(val, 100);
                int current = active_readers.fetch_add(1, std::memory_order_acq_rel) + 1;

                // Фиксируем пиковое количество одновременных читателей
                int prev_max = max_simultaneous_readers.load(std::memory_order_relaxed);
                while (current > prev_max &&
                       !max_simultaneous_readers.compare_exchange_weak(prev_max, current)) {
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(30));
                active_readers.fetch_sub(1, std::memory_order_release);
            });
        });
    }

    for (auto& t : readers) {
        t.join();
    }

    // При использовании обычного std::mutex здесь всегда была бы 1.
    // С std::shared_mutex читатели заходят в критическую секцию одновременно.
    EXPECT_GT(max_simultaneous_readers.load(), 1);
}

TEST(ReadWriteLockTest, GetOrComputeInvokesFactoryOnceUnderContention) {
    arch::ThreadSafeCache<std::string, int> cache;
    std::atomic<int> compute_calls{0};

    constexpr int kThreads = 8;
    std::vector<std::thread> threads;

    for (int i = 0; i < kThreads; ++i) {
        threads.emplace_back([&]() {
            int result = cache.get_or_compute("expensive_key", [&]() {
                compute_calls.fetch_add(1, std::memory_order_relaxed);
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
                return 777;
            });
            EXPECT_EQ(result, 777);
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(compute_calls.load(), 1);
}

TEST(ReadWriteLockTest, ConcurrentReadersAndWritersStress) {
    arch::ThreadSafeCache<int, int> cache;
    constexpr int kKeys = 10;
    for (int i = 0; i < kKeys; ++i) {
        cache.set(i, i * 10);
    }

    std::atomic<bool> stop{false};
    std::vector<std::thread> threads;

    // 4 читателя
    for (int r = 0; r < 4; ++r) {
        threads.emplace_back([&]() {
            while (!stop.load(std::memory_order_relaxed)) {
                for (int k = 0; k < kKeys; ++k) {
                    auto v = cache.get(k);
                    EXPECT_TRUE(v.has_value());
                }
            }
        });
    }

    // 2 писателя
    for (int w = 0; w < 2; ++w) {
        threads.emplace_back([&]() {
            for (int iter = 0; iter < 200; ++iter) {
                cache.set(iter % kKeys, iter);
            }
        });
    }

    // Ждем завершения писателей
    threads[4].join();
    threads[5].join();
    stop.store(true, std::memory_order_relaxed);

    for (int r = 0; r < 4; ++r) {
        threads[r].join();
    }
}
