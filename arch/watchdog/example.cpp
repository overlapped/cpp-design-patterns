#include "watchdog.hpp"
#include <cassert>
#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;
    using Ms = milliseconds;
    using TP = steady_clock::time_point;

    Watchdog<steady_clock> wd(Ms(100));

    TP t0{Ms(0)};
    wd.start(t0);
    assert(wd.started());
    assert(!wd.expired(t0));
    assert(!wd.expired(t0 + Ms(50)));

    // Пнули — дедлайн сдвинулся
    wd.kick(t0 + Ms(50));
    assert(!wd.expired(t0 + Ms(100)));
    assert(!wd.expired(t0 + Ms(149)));
    assert(wd.expired(t0 + Ms(150)));

    // Идемпотентность: повторный вызов не меняет состояние
    assert(wd.expired(t0 + Ms(200)));
    assert(wd.expired(t0 + Ms(1000)));

    // remaining()
    wd.start(t0);
    assert(wd.remaining(t0 + Ms(30)) == Ms(70));

    std::cout << "Watchdog example OK\n";
    return 0;
}