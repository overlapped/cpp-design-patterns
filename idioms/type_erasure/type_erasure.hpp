#pragma once
#include <memory>
#include <type_traits>
#include <utility>

// Type Erasure: храним объекты разных типов за общим интерфейсом
// БЕЗ наследования от общего базового класса. Внутренний Concept —
// это скрытый полиморфизм, который компилятор генерирует через шаблон.
//
// Внешне класс ведёт себя как value-тип: его можно копировать,
// перемещать, хранить в контейнерах.

class AnyDrawable {
public:
    // Публичный интерфейс: только то, что нужно клиенту.
    void draw() const { self_->draw(); }

    // Конструктор из любого типа, у которого есть метод draw()
    template<typename T,
             typename = std::enable_if_t<
                 !std::is_same_v<std::decay_t<T>, AnyDrawable> &&
                 std::is_invocable_v<decltype(&T::draw), const T*>>>
    AnyDrawable(T&& obj)
        : self_(std::make_unique<Model<std::decay_t<T>>>(std::forward<T>(obj))) {}

    // Rule of Five — потому что владеем unique_ptr<Concept>
    AnyDrawable(const AnyDrawable& other)
        : self_(other.self_->clone()) {}

    AnyDrawable(AnyDrawable&& other) noexcept = default;

    AnyDrawable& operator=(const AnyDrawable& other) {
        if (this != &other) {
            self_ = other.self_->clone();
        }
        return *this;
    }

    AnyDrawable& operator=(AnyDrawable&& other) noexcept = default;

    ~AnyDrawable() = default;

private:
    struct Concept {
        virtual ~Concept() = default;
        virtual void draw() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };

    template<typename T>
    struct Model : Concept {
        explicit Model(T obj) : obj_(std::move(obj)) {}
        void draw() const override { obj_.draw(); }
        std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Model>(*this);
        }
        T obj_;
    };

    std::unique_ptr<Concept> self_;
};