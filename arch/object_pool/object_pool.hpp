#pragma once
#include <memory>
#include <vector>
#include <stdexcept>
#include <functional>

// Object Pool: пул готовых к использованию объектов. Клиент берёт
// объект из пула (acquire), использует и возвращает (release).
// В отличие от Memory Pool (пул сырой памяти), здесь хранятся
// сконструированные объекты, готовые к работе.
//
// Классический пример — пул соединений с БД: создавать соединение
// дорого, поэтому их создают заранее и переиспользуют.

template<typename T>
class ObjectPool {
public:
    // Фабрика: как создавать новый объект, если пул пуст
    using Factory = std::function<std::unique_ptr<T>()>;

    explicit ObjectPool(std::size_t initialSize, Factory factory)
        : factory_(std::move(factory)) {
        for (std::size_t i = 0; i < initialSize; ++i) {
            free_.push_back(factory_());
        }
    }

    // Взять объект из пула. Если пул пуст — создать новый.
    std::unique_ptr<T> acquire() {
        if (free_.empty()) {
            return factory_();
        }
        auto obj = std::move(free_.back());
        free_.pop_back();
        return obj;
    }

    // Вернуть объект в пул. Владение переходит обратно пулу.
    void release(std::unique_ptr<T> obj) {
        if (obj) {
            free_.push_back(std::move(obj));
        }
    }

    // Сколько объектов сейчас свободно
    std::size_t available() const { return free_.size(); }

    // Сколько всего объектов было создано (свободных + выданных)
    std::size_t total() const { return free_.size() + inUse_; }

private:
    Factory factory_;
    std::vector<std::unique_ptr<T>> free_;
    std::size_t inUse_ = 0;
};