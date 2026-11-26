#pragma once
#include <atomic>
#include <cstdint>
#include <thread>
#include <vector>

// std::atomic_ref (C++20): атомарный доступ к НЕатомарной переменной.
// Полезно, когда структура данных в основном используется неатомарно,
// но в одном месте (например, в статистике) требует атомарности.
//
// В отличие от std::atomic<T>, atomic_ref не владеет переменной —
// он лишь даёт атомарный «вид» на неё. Это позволяет:
// - хранить данные в обычной структуре (без накладных расходов atomic);
// - в нужный момент делать атомарные операции через atomic_ref.

// 1. Счётчик в структуре: поле обычное int, но иногда нужно
//    атомарно инкрементировать из нескольких потоков.
struct Stats {
    int hits = 0;      // обычное поле — быстрое чтение в однопоточном коде
    int misses = 0;
};

// Атомарный инкремент через atomic_ref
inline void recordHit(Stats& s) {
    std::atomic_ref<int> ref(s.hits);
    ref.fetch_add(1, std::memory_order_relaxed);
}

inline void recordMiss(Stats& s) {
    std::atomic_ref<int> ref(s.misses);
    ref.fetch_add(1, std::memory_order_relaxed);
}

// 2. Флаг «готово»: обычный bool, но проверяется из другого потока
struct Worker {
    bool done = false;    // обычное поле
    int result = 0;
};

inline void runWorker(Worker& w) {
    // имитация работы
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    w.result = 42;
    // атомарно выставляем флаг
    std::atomic_ref<bool> ref(w.done);
    ref.store(true, std::memory_order_release);
}

inline bool isDone(Worker& w) {
    std::atomic_ref<bool> ref(w.done);
    return ref.load(std::memory_order_acquire);
}

// 3. Счётчик с CAS: атомарный compare_exchange через atomic_ref
struct Counter {
    int value = 0;
};

inline bool tryIncrementIfLess(Counter& c, int limit) {
    std::atomic_ref<int> ref(c.value);
    int expected = ref.load(std::memory_order_relaxed);
    while (expected < limit) {
        if (ref.compare_exchange_weak(expected, expected + 1,
                                      std::memory_order_relaxed)) {
            return true;
        }
        // expected обновлён при неудаче — пробуем снова
    }
    return false;
}