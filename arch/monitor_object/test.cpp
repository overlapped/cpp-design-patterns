#include "monitor_object.hpp"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <numeric>
#include <stdexcept>
#include <thread>
#include <vector>

TEST(MonitorObjectTest, ThrowsOnZeroCapacity) {
    EXPECT_THROW(arch::BoundedMonitorQueue<int>(0), std::invalid_argument);
}

TEST(MonitorObjectTest, BasicPushAndPopFIFO) {
    arch::BoundedMonitorQueue<int> queue(3);

    EXPECT_TRUE(queue.push(10));
    EXPECT_TRUE(queue.push(20));
    EXPECT_EQ(queue.size(), 2u);

    EXPECT_EQ(queue.pop(), 10);
    EXPECT_EQ(queue.pop(), 20);
    EXPECT_TRUE(queue.empty());
}

TEST(MonitorObjectTest, BlocksProducerWhenFull) {
    arch::BoundedMonitorQueue<int> queue(2);
    ASSERT_TRUE(queue.push(1));
    ASSERT_TRUE(queue.push(2));

    std::atomic<bool> third_pushed{false};

    std::thread producer([&]() {
        queue.push(3);
        third_pushed.store(true, std::memory_order_release);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    // Очередь полна, третий push должен висеть в ожидании
    EXPECT_FALSE(third_pushed.load(std::memory_order_acquire));

    // Освобождаем место
    EXPECT_EQ(queue.pop(), 1);
    producer.join();

    EXPECT_TRUE(third_pushed.load(std::memory_order_acquire));
    EXPECT_EQ(queue.pop(), 2);
    EXPECT_EQ(queue.pop(), 3);
}

TEST(MonitorObjectTest, CloseUnblocksWaitingThreadsAndDrainsRemaining) {
    arch::BoundedMonitorQueue<int> queue(5);
    ASSERT_TRUE(queue.push(100));

    queue.close();

    // Новые элементы после закрытия не принимаются
    EXPECT_FALSE(queue.push(200));

    // Оставшиеся элементы можно дочитать
    EXPECT_EQ(queue.pop(), 100);

    // Когда очередь опустела и закрыта — возвращается std::nullopt
    EXPECT_EQ(queue.pop(), std::nullopt);
}

TEST(MonitorObjectTest, TryPopForTimesOut) {
    arch::BoundedMonitorQueue<int> queue(2);

    auto res = queue.try_pop_for(std::chrono::milliseconds(20));
    EXPECT_FALSE(res.has_value());
}

TEST(MonitorObjectTest, MultiProducerMultiConsumerStress) {
    constexpr int kProducers = 4;
    constexpr int kConsumers = 4;
    constexpr int kItemsPerProducer = 250;

    arch::BoundedMonitorQueue<int> queue(16);
    std::atomic<int> consumed_count{0};
    std::atomic<long long> total_sum{0};

    std::vector<std::thread> producers;
    for (int p = 0; p < kProducers; ++p) {
        producers.emplace_back([&queue]() {
            for (int i = 1; i <= kItemsPerProducer; ++i) {
                queue.push(i);
            }
        });
    }

    std::vector<std::thread> consumers;
    for (int c = 0; c < kConsumers; ++c) {
        consumers.emplace_back([&]() {
            while (auto val = queue.pop()) {
                consumed_count.fetch_add(1, std::memory_order_relaxed);
                total_sum.fetch_add(*val, std::memory_order_relaxed);
            }
        });
    }

    for (auto& p : producers) {
        p.join();
    }
    queue.close();

    for (auto& c : consumers) {
        c.join();
    }

    const int expected_count = kProducers * kItemsPerProducer;
    const long long expected_sum =
        static_cast<long long>(kProducers) * (kItemsPerProducer * (kItemsPerProducer + 1) / 2);

    EXPECT_EQ(consumed_count.load(std::memory_order_relaxed), expected_count);
    EXPECT_EQ(total_sum.load(std::memory_order_relaxed), expected_sum);
}
