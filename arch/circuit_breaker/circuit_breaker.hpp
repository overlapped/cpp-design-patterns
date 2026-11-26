#pragma once
#include <chrono>
#include <functional>
#include <stdexcept>
#include <string>

// Circuit Breaker: паттерн отказоустойчивости. Если сервис начинает
// падать, «предохранитель» размыкается и запросы временно не идут
// к нему — быстрый fail вместо таймаута. Через timeout предохранитель
// переходит в HalfOpen и пробует один запрос: если успешно —
// замыкается, если нет — снова размыкается.
//
// Состояния:
//   Closed    — всё работает, запросы проходят.
//   Open      — сервис падает, запросы сразу отклоняются.
//   HalfOpen  — пробный период: один запрос решает судьбу.

class CircuitBreaker {
public:
    enum class State { Closed, Open, HalfOpen };

    using Clock = std::chrono::steady_clock;

    CircuitBreaker(int failureThreshold,
                   std::chrono::milliseconds timeout)
        : failureThreshold_(failureThreshold),
          timeout_(timeout) {}

    // Выполнить операцию через предохранитель.
    // Если цепь разомкнута — бросает std::runtime_error сразу.
    // Если операция бросила — увеличивает счётчик ошибок.
    template<typename Fn>
    auto call(Fn&& fn) -> decltype(fn()) {
        if (state_ == State::Open) {
            if (Clock::now() - openedAt_ < timeout_) {
                throw std::runtime_error("circuit breaker is open");
            }
            // timeout истёк — пробуем HalfOpen
            state_ = State::HalfOpen;
        }

        try {
            auto result = fn();
            onSuccess();
            return result;
        } catch (...) {
            onFailure();
            throw;
        }
    }

    State state() const { return state_; }
    int failureCount() const { return failureCount_; }

private:
    void onSuccess() {
        failureCount_ = 0;
        state_ = State::Closed;
    }

    void onFailure() {
        ++failureCount_;
        if (state_ == State::HalfOpen ||
            failureCount_ >= failureThreshold_) {
            state_ = State::Open;
            openedAt_ = Clock::now();
        }
    }

    int failureThreshold_;
    std::chrono::milliseconds timeout_;
    State state_ = State::Closed;
    int failureCount_ = 0;
    Clock::time_point openedAt_{};
};