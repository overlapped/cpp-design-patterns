#include "circuit_breaker.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <thread>

int main() {
    CircuitBreaker cb(2, std::chrono::milliseconds(50));

    // Успешный вызов — цепь замкнута
    int ok = cb.call([] { return 42; });
    assert(ok == 42);
    assert(cb.state() == CircuitBreaker::State::Closed);

    // Два падения — цепь размыкается
    for (int i = 0; i < 2; ++i) {
        try {
            cb.call([]() -> int { throw std::runtime_error("boom"); });
        } catch (const std::runtime_error&) {}
    }
    assert(cb.state() == CircuitBreaker::State::Open);

    // Пока цепь разомкнута — запросы сразу отклоняются
    bool rejected = false;
    try {
        cb.call([] { return 1; });
    } catch (const std::runtime_error& e) {
        rejected = true;
    }
    assert(rejected);

    // Ждём timeout — цепь переходит в HalfOpen
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    int recovered = cb.call([] { return 7; });
    assert(recovered == 7);
    assert(cb.state() == CircuitBreaker::State::Closed);

    std::cout << "Circuit Breaker: fast fail when service is down\n";
    return 0;
}