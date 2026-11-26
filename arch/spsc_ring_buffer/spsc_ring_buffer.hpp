#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <new>
#include <optional>
#include <type_traits>
#include <utility>

namespace arch {

// Lock-free / Wait-free циклический буфер для одного писателя (Producer)
// и одного читателя (Consumer).
//
// Ключевые оптимизации:
// 1. Capacity — степень двойки: индексация через побитовое И (& kMask) вместо деления (%).
// 2. alignas(kCacheLineSize) для head_ и tail_: устраняет False Sharing между ядрами CPU.
// 3. Строгие барьеры memory_order_acquire / memory_order_release без использования seq_cst.
template <typename T, std::size_t Capacity>
class SpscRingBuffer {
    static_assert(Capacity >= 2, "Capacity must be at least 2");
    static_assert((Capacity & (Capacity - 1)) == 0, "Capacity must be a power of 2");
    static_assert(std::is_nothrow_move_constructible_v<T> || std::is_nothrow_copy_constructible_v<T>,
                  "T must be nothrow move or copy constructible");

public:
    SpscRingBuffer() = default;

    SpscRingBuffer(const SpscRingBuffer&) = delete;
    SpscRingBuffer& operator=(const SpscRingBuffer&) = delete;
    SpscRingBuffer(SpscRingBuffer&&) = delete;
    SpscRingBuffer& operator=(SpscRingBuffer&&) = delete;

    // Вызывается ТОЛЬКО из потока-производителя (Producer).
    // Возвращает false, если буфер заполнен.
    [[nodiscard]] bool try_push(T item) noexcept {
        const std::size_t current_tail = tail_.load(std::memory_order_relaxed);
        const std::size_t next_tail = (current_tail + 1) & kMask;

        // Синхронизируемся с consumer'ом: проверяем, не догнал ли tail голову head_
        if (next_tail == head_.load(std::memory_order_acquire)) {
            return false; // Буфер полон (1 слот остаётся разделителем)
        }

        buffer_[current_tail] = std::move(item);

        // Публикуем запись элемента в buffer_ до обновления tail_
        tail_.store(next_tail, std::memory_order_release);
        return true;
    }

    // Вызывается ТОЛЬКО из потока-потребителя (Consumer).
    // Возвращает std::nullopt, если буфер пуст.
    [[nodiscard]] std::optional<T> try_pop() noexcept {
        const std::size_t current_head = head_.load(std::memory_order_relaxed);

        // Синхронизируемся с producer'ом: проверяем наличие новых элементов
        if (current_head == tail_.load(std::memory_order_acquire)) {
            return std::nullopt; // Буфер пуст
        }

        T item = std::move(buffer_[current_head]);

        // Публикуем освобождение слота до обновления head_
        head_.store((current_head + 1) & kMask, std::memory_order_release);
        return item;
    }

    [[nodiscard]] bool empty() const noexcept {
        return head_.load(std::memory_order_acquire) == tail_.load(std::memory_order_acquire);
    }

    [[nodiscard]] std::size_t size() const noexcept {
        const std::size_t head = head_.load(std::memory_order_acquire);
        const std::size_t tail = tail_.load(std::memory_order_acquire);
        return (tail - head) & kMask;
    }

    // Реальная полезная ёмкость на 1 меньше Capacity, так как 1 слот используется
    // для различения состояний «пусто» (head == tail) и «полно» ((tail + 1) & mask == head).
    [[nodiscard]] static constexpr std::size_t usable_capacity() noexcept {
        return Capacity - 1;
    }

private:
    static constexpr std::size_t kMask = Capacity - 1;

#ifdef __cpp_lib_hardware_interference_size
    static constexpr std::size_t kCacheLineSize = std::hardware_destructive_interference_size;
#else
    static constexpr std::size_t kCacheLineSize = 64;
#endif

    // Разносим head_ и tail_ по разным кэш-линиям, чтобы запись в tail_
    // на одном ядре CPU не инвалидировала кэш-линию с head_ на другом ядре.
    alignas(kCacheLineSize) std::atomic<std::size_t> head_{0};
    alignas(kCacheLineSize) std::atomic<std::size_t> tail_{0};

    alignas(kCacheLineSize) std::array<T, Capacity> buffer_{};
};

} // namespace arch
