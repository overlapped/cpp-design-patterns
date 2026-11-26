#include "jthread_stop_token.hpp"
#include <cassert>
#include <iostream>

int main() {
    // 1. SimpleWorker: деструктор сам остановит поток
    {
        SimpleWorker w;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        assert(w.counter() > 0);
    }  // ~SimpleWorker -> jthread::~jthread -> request_stop + join

    // 2. QueueWorker: ждёт задачи, но просыпается по stop
    {
        QueueWorker w;
        w.push(1);
        w.push(2);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        assert(w.processed() >= 2);
    }  // ~QueueWorker -> request_stop -> wait пробуждается -> join

    // 3. ParallelWorkers: stopAll останавливает всех сразу
    {
        ParallelWorkers pw(3);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        pw.stopAll();
        // Деструктор всё равно вызовет request_stop + join, но потоки уже остановлены
    }

    std::cout << "jthread + stop_token: cooperative cancellation\n";
    return 0;
}