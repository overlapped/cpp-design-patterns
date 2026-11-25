#pragma once
#include <iterator>
#include <utility>

// Теги — пустые структуры, которые несут информацию о стратегии.
struct fast_tag {};
struct slow_tag {};

// Простая функция, которая «выбирает» реализацию по тегу.
// В реальном коде тег обычно получается из traits типа-контейнера.
template<typename Container>
void processImpl(Container& c, fast_tag) {
    // Быстрый путь: например, работаем с непрерывным буфером
    for (auto& x : c) x += 1;
}

template<typename Container>
void processImpl(Container& c, slow_tag) {
    // Медленный путь: например, доступ через итераторы с проверками
    for (auto it = c.begin(); it != c.end(); ++it) *it += 1;
}

// Диспетчер: выбираем тег на этапе компиляции по traits.
template<typename Container>
struct container_traits {
    // По умолчанию — медленный путь. Специализации могут переопределить.
    using tag = slow_tag;
};

// Специализация для «быстрого» контейнера (условно — vector)
#include <vector>
template<typename T, typename A>
struct container_traits<std::vector<T, A>> {
    using tag = fast_tag;
};

template<typename Container>
void process(Container& c) {
    processImpl(c, typename container_traits<Container>::tag{});
}