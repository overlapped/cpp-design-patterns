#pragma once

#include <ostream>
#include <utility>

namespace idioms {

// ============================================================================
// 1. Классический подход (НЕ Hidden Friend — функция объявлена в namespace)
// ============================================================================
class ClassicNumber {
public:
    // Не-explicit конструктор для демонстрации ловушки неявных преобразований
    ClassicNumber(int value) noexcept : value_(value) {} // NOLINT(google-explicit-constructor)

    [[nodiscard]] int value() const noexcept { return value_; }

private:
    int value_;
};

// Оператор объявлен в области видимости namespace idioms:
// 1. Компилятор добавляет его в глобальный набор кандидатов при ЛЮБОМ вызове `a + b`
//    внутри namespace idioms (или при `using namespace idioms`), замедляя компиляцию.
// 2. Разрешает странные выражения вроде `5 + ClassicNumber(10)`, так как для свободной
//    функции неявное преобразование работает для ОБОИХ аргументов.
[[nodiscard]] inline ClassicNumber operator+(const ClassicNumber& lhs,
                                             const ClassicNumber& rhs) noexcept {
    return ClassicNumber{lhs.value() + rhs.value()};
}

// ============================================================================
// 2. Идиома Hidden Friend (Скрытый друг)
// ============================================================================
class ModernNumber {
public:
    ModernNumber(int value) noexcept : value_(value) {} // NOLINT(google-explicit-constructor)

    [[nodiscard]] int value() const noexcept { return value_; }

    // Оператор определён ВНУТРИ тела класса с ключевым словом `friend`.
    // Несмотря на `friend`, это НЕ метод класса, а свободная функция в namespace idioms,
    // НО с важнейшим свойством: она невидима для обычного поиска имён (unqualified lookup)
    // и находится ТОЛЬКО через ADL (Argument-Dependent Lookup), когда хотя бы один
    // из аргументов имеет тип ModernNumber!
    [[nodiscard]] friend ModernNumber operator+(const ModernNumber& lhs,
                                                const ModernNumber& rhs) noexcept {
        return ModernNumber{lhs.value() + rhs.value()};
    }

    [[nodiscard]] friend bool operator==(const ModernNumber& lhs,
                                         const ModernNumber& rhs) noexcept {
        return lhs.value_ == rhs.value_;
    }

    friend std::ostream& operator<<(std::ostream& os, const ModernNumber& num) {
        return os << "ModernNumber(" << num.value_ << ')';
    }

private:
    int value_;
};

// ============================================================================
// 3. Дополнительный эффект: защита от нежелательных неявных преобразований
// ============================================================================
// Если класс не имеет неявного конструктора от int (или мы хотим запретить `5 == obj`),
// но при этом определить `operator==` внутри класса как hidden friend с параметрами
// (const StrictId&, const StrictId&):
//
// Для `StrictId(5) == 5` -> ADL находит hidden friend по первому аргументу `StrictId`,
// а второй аргумент `5` приводится к `StrictId` (если конструктор не explicit).
// НО если конструктор `explicit`, то даже `StrictId(5) == 5` не скомпилируется,
// а для посторонних типов `operator==` вообще не участвует в разрешении перегрузок!
class StrictId {
public:
    explicit StrictId(int id) noexcept : id_(id) {}

    [[nodiscard]] int get() const noexcept { return id_; }

    // Hidden friend: доступен ТОЛЬКО если среди аргументов есть StrictId
    [[nodiscard]] friend bool operator==(const StrictId& lhs, const StrictId& rhs) noexcept {
        return lhs.id_ == rhs.id_;
    }

    [[nodiscard]] friend bool operator!=(const StrictId& lhs, const StrictId& rhs) noexcept {
        return !(lhs == rhs);
    }

private:
    int id_;
};

// Вспомогательная шаблонная функция для проверки того, что скрытый друг
// НЕ виден при обычном поиске без ADL (например, через указатель на функцию).
template <typename T>
concept HasAdlAddition = requires(T a, T b) {
    { a + b };
};

} // namespace idioms
