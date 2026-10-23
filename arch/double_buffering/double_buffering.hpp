#pragma once
#include <array>
#include <cstddef>
#include <utility>

// Double Buffering: два буфера фиксированного размера.
// Пишем всегда в back(), читаем всегда из front().
// swap() меняет местами — типично вызывается по событию
// «кадр готов» / «DMA завершён».
template<typename T, std::size_t N>
class DoubleBuffer {
public:
    using Buffer = std::array<T, N>;

    // Доступ к буферу, в который сейчас пишут
    Buffer& back() { return buffers_[back_idx_]; }
    const Buffer& back() const { return buffers_[back_idx_]; }

    // Доступ к буферу, который сейчас читают (последний готовый)
    Buffer& front() { return buffers_[front_idx_]; }
    const Buffer& front() const { return buffers_[front_idx_]; }

    // Переключить: готовый back становится front, старый front — back.
    // Содержимое старого front НЕ очищается: перезапись — ответственность
    // писателя (так эффективнее: не тратим циклы на затирание).
    void swap() noexcept {
        std::swap(front_idx_, back_idx_);
    }

    static constexpr std::size_t size() { return N; }

    // Индексы полезны для отладки и тестов
    std::size_t front_index() const { return front_idx_; }
    std::size_t back_index() const { return back_idx_; }

private:
    Buffer buffers_[2]{};
    std::size_t front_idx_ = 0;
    std::size_t back_idx_ = 1;
};