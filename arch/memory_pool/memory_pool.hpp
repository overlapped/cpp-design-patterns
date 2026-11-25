#pragma once
#include <cstddef>
#include <cstdint>
#include <new>
#include <type_traits>
#include <utility>

// Пул блоков фиксированного размера. Память выделяется один раз
// в конструкторе (буфер на стеке или в статике), дальше — только
// выдача/возврат указателей из free-list. Никаких new/delete в runtime.
//
// Требования к T: BlockSize >= sizeof(T) и alignment >= alignof(T).
template <typename T, std::size_t BlockCount>
class MemoryPool {
    static_assert(BlockCount > 0, "BlockCount must be > 0");

   public:
    MemoryPool() {
        // Собираем free-list: каждый блок указывает на следующий
        for (std::size_t i = 0; i < BlockCount; ++i) {
            free_[i].next = (i + 1 < BlockCount) ? &free_[i + 1] : nullptr;
        }
        head_ = &free_[0];
    }

    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    // Создать объект в пуле. Возвращает nullptr, если блоков нет.
    template <typename... Args>
    T* create(Args&&... args) {
        if (!head_)
            return nullptr;
        Node* node = head_;
        head_ = head_->next;
        T* obj = reinterpret_cast<T*>(node->storage);
        new (obj) T(std::forward<Args>(args)...);
        return obj;
    }

    // Вернуть объект в пул. Деструктор вызывается автоматически.
    void destroy(T* obj) {
        if (!obj)
            return;
        obj->~T();
        Node* node = reinterpret_cast<Node*>(obj);
        node->next = head_;
        head_ = node;
    }

    std::size_t available() const {
        std::size_t n = 0;
        for (Node* p = head_; p; p = p->next) ++n;
        return n;
    }

    static constexpr std::size_t capacity() { return BlockCount; }

   private:
    union Node {
        Node* next;
        alignas(T) std::byte storage[sizeof(T)];
    };

    Node free_[BlockCount];
    Node* head_ = nullptr;
};
