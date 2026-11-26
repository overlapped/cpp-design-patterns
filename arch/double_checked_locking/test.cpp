#include "double_checked_locking.hpp"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <stdexcept>
#include <thread>
#include <vector>

namespace {

struct Payload {
    int a;
    int b;
    int c;

    Payload(int x, int y, int z) : a(x), b(y), c(z) {}
};

} // namespace

TEST(DoubleCheckedLockingTest, LazyInitializationHappensOnlyOnFirstGet) {
    int creations = 0;
    arch::LazyDclp<Payload> lazy([&creations]() {
        ++creations;
        return std::make_unique<Payload>(1, 2, 3);
    });

    EXPECT_FALSE(lazy.is_initialized());
    EXPECT_EQ(creations, 0);

    const Payload& p1 = lazy.get();
    EXPECT_TRUE(lazy.is_initialized());
    EXPECT_EQ(creations, 1);
    EXPECT_EQ(p1.a, 1);
    EXPECT_EQ(p1.b, 2);
    EXPECT_EQ(p1.c, 3);

    // Повторный вызов возвращает тот же самый экземпляр без пересоздания
    const Payload& p2 = lazy.get();
    EXPECT_EQ(&p1, &p2);
    EXPECT_EQ(creations, 1);
}

TEST(DoubleCheckedLockingTest, InitializesExactlyOnceUnderHighContention) {
    constexpr int kThreads = 16;
    std::atomic<int> factory_calls{0};

    arch::LazyDclp<Payload> lazy([&factory_calls]() {
        factory_calls.fetch_add(1, std::memory_order_relaxed);
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        return std::make_unique<Payload>(10, 20, 30);
    });

    std::vector<std::thread> threads;
    std::vector<const Payload*> pointers(kThreads, nullptr);

    for (int i = 0; i < kThreads; ++i) {
        threads.emplace_back([&lazy, &pointers, i]() {
            const Payload& ref = lazy.get();
            // Проверяем, что все поля полностью проинициализированы (нет torn reads)
            EXPECT_EQ(ref.a, 10);
            EXPECT_EQ(ref.b, 20);
            EXPECT_EQ(ref.c, 30);
            pointers[i] = &ref;
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(factory_calls.load(std::memory_order_relaxed), 1);
    for (int i = 1; i < kThreads; ++i) {
        EXPECT_EQ(pointers[0], pointers[i]);
    }
}

TEST(DoubleCheckedLockingTest, ExceptionSafetyOnFactoryFailure) {
    int attempts = 0;
    arch::LazyDclp<int> lazy([&attempts]() -> std::unique_ptr<int> {
        ++attempts;
        if (attempts == 1) {
            throw std::runtime_error("Initialization failed");
        }
        return std::make_unique<int>(42);
    });

    // Первая попытка падает с исключением, объект остаётся неинициализированным
    EXPECT_THROW((void)lazy.get(), std::runtime_error);
    EXPECT_FALSE(lazy.is_initialized());

    // Вторая попытка успешно завершает инициализацию
    EXPECT_EQ(lazy.get(), 42);
    EXPECT_TRUE(lazy.is_initialized());
    EXPECT_EQ(attempts, 2);
}

TEST(DoubleCheckedLockingTest, CallOnceAlternativeWorksIdentically) {
    std::atomic<int> calls{0};
    arch::LazyCallOnce<Payload> lazy([&calls]() {
        calls.fetch_add(1, std::memory_order_relaxed);
        return std::make_unique<Payload>(7, 8, 9);
    });

    std::vector<std::thread> threads;
    for (int i = 0; i < 8; ++i) {
        threads.emplace_back([&lazy]() {
            const auto& p = lazy.get();
            EXPECT_EQ(p.a + p.b + p.c, 24);
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(calls.load(), 1);
}
