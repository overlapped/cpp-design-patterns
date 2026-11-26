#include "spsc_ring_buffer.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <thread>

TEST(SpscRingBufferTest, EmptyAndCapacityProperties) {
    arch::SpscRingBuffer<int, 4> rb;

    EXPECT_TRUE(rb.empty());
    EXPECT_EQ(rb.size(), 0u);
    EXPECT_EQ(rb.usable_capacity(), 3u);
    EXPECT_EQ(rb.try_pop(), std::nullopt);
}

TEST(SpscRingBufferTest, PushPopAndFullCondition) {
    arch::SpscRingBuffer<int, 4> rb; // полезная ёмкость = 3

    EXPECT_TRUE(rb.try_push(10));
    EXPECT_TRUE(rb.try_push(20));
    EXPECT_TRUE(rb.try_push(30));
    EXPECT_EQ(rb.size(), 3u);

    // 4-й элемент не должен поместиться
    EXPECT_FALSE(rb.try_push(40));

    EXPECT_EQ(rb.try_pop(), 10);
    EXPECT_EQ(rb.size(), 2u);

    // После освобождения слота снова можно писать (проверка wrap-around)
    EXPECT_TRUE(rb.try_push(40));
    EXPECT_EQ(rb.try_pop(), 20);
    EXPECT_EQ(rb.try_pop(), 30);
    EXPECT_EQ(rb.try_pop(), 40);
    EXPECT_TRUE(rb.empty());
}

TEST(SpscRingBufferTest, WrapAroundMultipleCycles) {
    arch::SpscRingBuffer<int, 4> rb;

    for (int cycle = 0; cycle < 10; ++cycle) {
        ASSERT_TRUE(rb.try_push(cycle * 2));
        ASSERT_TRUE(rb.try_push(cycle * 2 + 1));

        EXPECT_EQ(rb.try_pop(), cycle * 2);
        EXPECT_EQ(rb.try_pop(), cycle * 2 + 1);
        EXPECT_TRUE(rb.empty());
    }
}

TEST(SpscRingBufferTest, HighThroughputConcurrentTransfer) {
    constexpr std::uint64_t kMessageCount = 500'000;
    arch::SpscRingBuffer<std::uint64_t, 1024> rb;

    std::thread producer([&rb]() {
        for (std::uint64_t i = 1; i <= kMessageCount; ++i) {
            while (!rb.try_push(i)) {
                std::this_thread::yield();
            }
        }
    });

    std::uint64_t expected_next = 1;
    std::uint64_t sum = 0;

    std::thread consumer([&]() {
        while (expected_next <= kMessageCount) {
            if (auto val = rb.try_pop()) {
                // Проверяем строгий порядок FIFO и отсутствие потерь/искажений
                ASSERT_EQ(*val, expected_next);
                sum += *val;
                ++expected_next;
            } else {
                std::this_thread::yield();
            }
        }
    });

    producer.join();
    consumer.join();

    EXPECT_TRUE(rb.empty());
    EXPECT_EQ(sum, kMessageCount * (kMessageCount + 1) / 2);
}
