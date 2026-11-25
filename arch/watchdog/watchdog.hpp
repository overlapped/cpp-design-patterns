#pragma once
#include <chrono>

// Watchdog с ручным управлением временем.
// time_point передаётся снаружи — это делает класс детерминированным
// и удобным для тестов. В реальном коде time_point берётся из
// std::chrono::steady_clock::now() (или тика аппаратного таймера).
template<typename Clock = std::chrono::steady_clock>
class Watchdog {
public:
    using Duration = typename Clock::duration;
    using TimePoint = typename Clock::time_point;

    explicit Watchdog(Duration timeout)
        : timeout_(timeout) {}

    // Запустить/перезапустить отсчёт с текущего момента
    void start(TimePoint now) {
        deadline_ = now + timeout_;
        started_ = true;
        expired_ = false;
    }

    // «Пнуть» watchdog: сбросить дедлайн от текущего момента
    void kick(TimePoint now) {
        deadline_ = now + timeout_;
        expired_ = false;
    }

    // Проверить, не истёк ли таймаут. Идемпотентно: повторный вызов
    // с тем же now вернёт то же значение и не сбросит expired_.
    bool expired(TimePoint now) {
        if (!started_) return false;
        if (!expired_ && now >= deadline_) expired_ = true;
        return expired_;
    }

    bool started() const { return started_; }
    Duration timeout() const { return timeout_; }

    // Для диагностики: сколько осталось до срабатывания
    Duration remaining(TimePoint now) const {
        if (!started_ || now >= deadline_) return Duration::zero();
        return deadline_ - now;
    }

private:
    Duration timeout_;
    TimePoint deadline_{};
    bool started_ = false;
    bool expired_ = false;
};