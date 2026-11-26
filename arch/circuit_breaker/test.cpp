#include <gtest/gtest.h>
#include "circuit_breaker.hpp"
#include <stdexcept>
#include <thread>

using namespace std::chrono_literals;

TEST(CircuitBreakerTest, StartsClosed) {
    CircuitBreaker cb(2, 50ms);
    EXPECT_EQ(cb.state(), CircuitBreaker::State::Closed);
}

TEST(CircuitBreakerTest, SuccessKeepsClosed) {
    CircuitBreaker cb(2, 50ms);
    EXPECT_EQ(cb.call([] { return 1; }), 1);
    EXPECT_EQ(cb.state(), CircuitBreaker::State::Closed);
}

TEST(CircuitBreakerTest, OpensAfterThreshold) {
    CircuitBreaker cb(2, 50ms);
    for (int i = 0; i < 2; ++i) {
        EXPECT_THROW(cb.call([]() -> int { throw std::runtime_error("x"); }),
                     std::runtime_error);
    }
    EXPECT_EQ(cb.state(), CircuitBreaker::State::Open);
}

TEST(CircuitBreakerTest, OpenRejectsWithoutCalling) {
    CircuitBreaker cb(1, 50ms);
    EXPECT_THROW(cb.call([]() -> int { throw std::runtime_error("x"); }),
                 std::runtime_error);
    EXPECT_EQ(cb.state(), CircuitBreaker::State::Open);

    // Пока open — функция не вызывается, сразу исключение
    bool called = false;
    EXPECT_THROW(cb.call([&] { called = true; return 1; }),
                 std::runtime_error);
    EXPECT_FALSE(called);
}

TEST(CircuitBreakerTest, HalfOpenRecoversOnSuccess) {
    CircuitBreaker cb(1, 20ms);
    EXPECT_THROW(cb.call([]() -> int { throw std::runtime_error("x"); }),
                 std::runtime_error);
    EXPECT_EQ(cb.state(), CircuitBreaker::State::Open);

    std::this_thread::sleep_for(30ms);   // timeout истёк
    EXPECT_EQ(cb.call([] { return 42; }), 42);
    EXPECT_EQ(cb.state(), CircuitBreaker::State::Closed);
}

TEST(CircuitBreakerTest, HalfOpenReopensOnFailure) {
    CircuitBreaker cb(1, 20ms);
    EXPECT_THROW(cb.call([]() -> int { throw std::runtime_error("x"); }),
                 std::runtime_error);

    std::this_thread::sleep_for(30ms);
    EXPECT_THROW(cb.call([]() -> int { throw std::runtime_error("y"); }),
                 std::runtime_error);
    EXPECT_EQ(cb.state(), CircuitBreaker::State::Open);
}