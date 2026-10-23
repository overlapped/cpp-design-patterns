#include "raii.hpp"
#include <cassert>
#include <cstdio>
#include <mutex>

int main() {
    // Файл закроется автоматически в конце области видимости.
    {
        FileHandle f("/tmp/raii_example.txt");
        f.write("hello");
        assert(f.isOpen());
    }  // ~FileHandle -> fclose

    // Мьютекс разблокируется даже при исключении.
    std::mutex m;
    try {
        ScopedLock lock(m);
        throw std::runtime_error("boom");
    } catch (const std::exception&) {
        // m уже разблокирован
    }

    // Проверяем, что мьютекс свободен
    bool acquired = m.try_lock();
    assert(acquired);
    m.unlock();

    std::puts("RAII example OK");
    return 0;
}