#pragma once

#include <concepts>
#include <cstddef>
#include <memory>
#include <new>
#include <string>
#include <type_traits>
#include <utility>

namespace idioms {

// Концепт определяет требования к любому типу T, который может быть обёрнут в Shape.
// Наследование от базового класса НЕ требуется (неинтрузивный полиморфизм).
template <typename T>
concept DrawableLike = std::copy_constructible<std::remove_cvref_t<T>> && requires(const T& obj) {
    { obj.draw() } -> std::convertible_to<std::string>;
    { obj.area() } -> std::convertible_to<double>;
};

// ============================================================================
// Полиморфная обёртка с семантикой значения (Value Semantics) + SBO
// ============================================================================
// Внутри используется паттерн Concept / Model (External Polymorphism)
// и Small Buffer Optimization (SBO): объекты размером <= kSboSize хранятся
// прямо на стеке внутри storage_, без обращения к куче (new/malloc).
class Shape {
public:
    static constexpr std::size_t kSboSize = 32;
    static constexpr std::size_t kSboAlign = alignof(std::max_align_t);

            template <typename T>
        requires(!std::same_as<std::remove_cvref_t<T>, Shape>) && DrawableLike<T>
    Shape(T&& value) { // NOLINT(google-explicit-constructor)
        using DecayedT = std::remove_cvref_t<T>;
        using ModelT = Model<DecayedT>;


        if constexpr (is_sbo_eligible<ModelT>()) {
            ::new (static_cast<void*>(storage_)) ModelT(std::forward<T>(value));
            concept_ = std::launder(reinterpret_cast<Concept*>(storage_));
            on_heap_ = false;
        } else {
            concept_ = new ModelT(std::forward<T>(value));
            on_heap_ = true;
        }
    }

    Shape(const Shape& other) {
        if (other.concept_) {
            other.concept_->clone_into(storage_, concept_, on_heap_);
        }
    }

    Shape& operator=(const Shape& other) {
        if (this != &other) {
            Shape tmp(other);
            swap(*this, tmp);
        }
        return *this;
    }

    Shape(Shape&& other) noexcept {
        if (other.concept_) {
            other.concept_->move_into(storage_, concept_, on_heap_);
            other.reset();
        }
    }

    Shape& operator=(Shape&& other) noexcept {
        if (this != &other) {
            reset();
            if (other.concept_) {
                other.concept_->move_into(storage_, concept_, on_heap_);
                other.reset();
            }
        }
        return *this;
    }

    ~Shape() {
        reset();
    }

    friend void swap(Shape& lhs, Shape& rhs) noexcept {
        if (&lhs == &rhs) {
            return;
        }
        Shape tmp(std::move(lhs));
        lhs = std::move(rhs);
        rhs = std::move(tmp);
    }

    // Публичный интерфейс, делегирующий вызовы стёртому типу
    [[nodiscard]] std::string draw() const {
        return concept_->draw();
    }

    [[nodiscard]] double area() const {
        return concept_->area();
    }

    // Вспомогательный метод для диагностики в тестах/примерах (показывает работу SBO)
    [[nodiscard]] bool is_stored_on_heap() const noexcept {
        return on_heap_;
    }

private:
    struct Concept {
        virtual ~Concept() = default;
        [[nodiscard]] virtual std::string draw() const = 0;
        [[nodiscard]] virtual double area() const = 0;
        virtual void clone_into(std::byte* buf, Concept*& out_ptr, bool& out_on_heap) const = 0;
        virtual void move_into(std::byte* buf, Concept*& out_ptr, bool& out_on_heap) noexcept = 0;
    };

    template <typename ModelT>
    static constexpr bool is_sbo_eligible() noexcept {
        return sizeof(ModelT) <= kSboSize && alignof(ModelT) <= kSboAlign &&
               std::is_nothrow_move_constructible_v<ModelT>;
    }

    template <typename T>
    struct Model final : Concept {
        explicit Model(T val) : data(std::move(val)) {}

        [[nodiscard]] std::string draw() const override {
            return data.draw();
        }

        [[nodiscard]] double area() const override {
            return data.area();
        }

        void clone_into(std::byte* buf, Concept*& out_ptr, bool& out_on_heap) const override {
            if constexpr (is_sbo_eligible<Model<T>>()) {
                ::new (static_cast<void*>(buf)) Model<T>(data);
                out_ptr = std::launder(reinterpret_cast<Concept*>(buf));
                out_on_heap = false;
            } else {
                out_ptr = new Model<T>(data);
                out_on_heap = true;
            }
        }

        void move_into(std::byte* buf, Concept*& out_ptr, bool& out_on_heap) noexcept override {
            if constexpr (is_sbo_eligible<Model<T>>()) {
                ::new (static_cast<void*>(buf)) Model<T>(std::move(data));
                out_ptr = std::launder(reinterpret_cast<Concept*>(buf));
                out_on_heap = false;
            } else {
                out_ptr = new Model<T>(std::move(data));
                out_on_heap = true;
            }
        }

        T data;
    };

    void reset() noexcept {
        if (!concept_) {
            return;
        }
        if (on_heap_) {
            delete concept_;
        } else {
            concept_->~Concept();
        }
        concept_ = nullptr;
        on_heap_ = false;
    }

    alignas(kSboAlign) std::byte storage_[kSboSize]{};
    Concept* concept_{nullptr};
    bool on_heap_{false};
};

} // namespace idioms
