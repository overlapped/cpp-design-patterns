#pragma once

#include <cstddef>
#include <functional>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <utility>

namespace arch {

// Демонстрация паттерна Read-Write Lock на базе std::shared_mutex.
// Позволяет множеству потоков-читателей (readers) одновременно обращаться к данным
// через std::shared_lock, но гарантирует эксклюзивный доступ для писателя (writer)
// через std::unique_lock.
template <typename Key, typename Value>
class ThreadSafeCache {
public:
    ThreadSafeCache() = default;

    ThreadSafeCache(const ThreadSafeCache&) = delete;
    ThreadSafeCache& operator=(const ThreadSafeCache&) = delete;
    ThreadSafeCache(ThreadSafeCache&&) = delete;
    ThreadSafeCache& operator=(ThreadSafeCache&&) = delete;

    // Операция чтения (Shared Lock):
    // Несколько потоков могут вызывать get() параллельно без блокировки друг друга.
    [[nodiscard]] std::optional<Value> get(const Key& key) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        auto it = storage_.find(key);
        if (it == storage_.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    // Операция чтения с трансформирующим функтором без копирования всего объекта наружу
    template <typename Func>
    bool read_with(const Key& key, Func&& reader_fn) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        auto it = storage_.find(key);
        if (it == storage_.end()) {
            return false;
        }
        std::invoke(std::forward<Func>(reader_fn), it->second);
        return true;
    }

    [[nodiscard]] bool contains(const Key& key) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return storage_.find(key) != storage_.end();
    }

    [[nodiscard]] std::size_t size() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return storage_.size();
    }

    // Операция записи (Exclusive / Unique Lock):
    // Блокирует всех читателей и других писателей на время модификации.
    void set(const Key& key, Value value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        storage_.insert_or_assign(key, std::move(value));
    }

    // Атомарная операция «прочитать или вычислить и записать» (Upgradable pattern):
    // Сначала проверяет под shared_lock, и только при промахе берет unique_lock.
    template <typename Factory>
    Value get_or_compute(const Key& key, Factory&& factory) {
        {
            std::shared_lock<std::shared_mutex> read_lock(mutex_);
            auto it = storage_.find(key);
            if (it != storage_.end()) {
                return it->second;
            }
        }

        // Ключ не найден: захватываем эксклюзивную блокировку для вставки.
        // Важно: между освобождением shared_lock и захватом unique_lock другой поток
        // мог уже вставить значение, поэтому нужна повторная проверка (double-check).
        std::unique_lock<std::shared_mutex> write_lock(mutex_);
        auto [it, inserted] = storage_.try_emplace(key, Value{});
        if (inserted) {
            it->second = std::invoke(std::forward<Factory>(factory));
        }
        return it->second;
    }

    bool erase(const Key& key) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        return storage_.erase(key) > 0;
    }

    void clear() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        storage_.clear();
    }

private:
    std::unordered_map<Key, Value> storage_;
    mutable std::shared_mutex mutex_;
};

} // namespace arch
