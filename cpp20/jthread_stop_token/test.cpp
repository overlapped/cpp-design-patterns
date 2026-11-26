#include <gtest/gtest.h>
#include "jthread_stop_token.hpp"
#include <thread>

using namespace std::chrono_literals;

TEST(JthreadStopTokenTest, SimpleWorkerRunsUntilDestroyed) {
    SimpleWorker w;
    std::this_thread::sleep_for(20ms);
    EXPECT_GT(w.counter(), 0);
    // Деструктор остановит поток — тест не должен висеть
}

TEST(JthreadStopTokenTest, QueueWorkerProcessesTasks) {
    QueueWorker w;
    w.push(1);
    w.push(2);
    w.push(3);
    std::this_thread::sleep_for(30ms);
    EXPECT_GE(w.processed(), 3);
}

TEST(JthreadStopTokenTest, QueueWorkerWakesOnStop) {
    QueueWorker w;
    // Не пушим задачи — поток висит на cv_.wait
    std::this_thread::sleep_for(20ms);
    // Деструктор вызовет request_stop, и wait проснётся
    // Если бы wait не поддерживал stop_token, тест завис бы
}

TEST(JthreadStopTokenTest, ParallelWorkersStopAll) {
    ParallelWorkers pw(3);
    std::this_thread::sleep_for(20ms);
    pw.stopAll();
    int c0 = pw.counter(0);
    int c1 = pw.counter(1);
    int c2 = pw.counter(2);
    EXPECT_GT(c0, 0);
    EXPECT_GT(c1, 0);
    EXPECT_GT(c2, 0);
    // Все три потока остановлены; деструктор просто join'ит
}

TEST(JthreadStopTokenTest, DestructorDoesNotHang) {
    // Если jthread не остановит поток, тест зависнет на деструкторе
    SimpleWorker w;
    std::this_thread::sleep_for(10ms);
    // ~SimpleWorker вызывается здесь
}