#include "object_pool.hpp"
#include <cassert>
#include <iostream>
#include <string>

// Дорогой в создании объект — соединение с «БД»
struct Connection {
    int id;
    bool busy = false;

    explicit Connection(int i) : id(i) {
        // имитация дорогого создания
    }

    std::string query(const std::string& sql) {
        busy = true;
        std::string result = "result for: " + sql;
        busy = false;
        return result;
    }
};

int main() {
    int nextId = 0;
    ObjectPool<Connection> pool(3, [&] {
        return std::make_unique<Connection>(nextId++);
    });

    assert(pool.available() == 3);

    // Берём объект из пула
    auto c1 = pool.acquire();
    assert(pool.available() == 2);
    assert(c1->query("SELECT 1") == "result for: SELECT 1");

    // Возвращаем — объект снова доступен
    pool.release(std::move(c1));
    assert(pool.available() == 3);

    // Берём больше, чем есть в пуле — пул создаёт новые
    auto a = pool.acquire();
    auto b = pool.acquire();
    auto c = pool.acquire();
    auto d = pool.acquire();   // пул пуст — создан новый
    assert(pool.available() == 0);

    pool.release(std::move(a));
    pool.release(std::move(b));
    pool.release(std::move(c));
    pool.release(std::move(d));
    assert(pool.available() == 4);

    std::cout << "Object Pool: reuse objects without re-creating\n";
    return 0;
}