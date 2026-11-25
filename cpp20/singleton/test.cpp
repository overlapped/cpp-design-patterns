#include <gtest/gtest.h>
#include "singleton.hpp"
#include <thread>
#include <vector>

TEST(SingletonTest, ThreadSafe) {
    std::vector<Singleton*> ptrs(16);
    std::vector<std::thread> threads;
    for (int i = 0; i < 16; ++i)
        threads.emplace_back([&, i] { ptrs[i] = &Singleton::instance(); });
    for (auto& t : threads) t.join();
    for (auto* p : ptrs) EXPECT_EQ(p, ptrs[0]);
}