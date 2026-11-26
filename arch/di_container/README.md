# DI Container

DI Container — архитектурный паттерн: контейнер зависимостей.
Позволяет регистрировать фабрики (как создавать объект) и получать
готовые экземпляры по типу. В отличие от compile-time подхода
(шаблонный контейнер с явными зависимостями), runtime-контейнер
хранит фабрики в `std::unordered_map<std::type_index, ...>` и умеет
разрешать зависимости по типу в runtime. Это упрощённая версия:
без автоматического разрешения графа зависимостей (как в Boost.DI),
но с явной регистрацией фабрик.

## Файлы

- `di_container.hpp` — `DIContainer` с `registerFactory<T>()`,
  `registerInstance<T>()`, `resolve<T>()`, `isRegistered<T>()`,
  `clear()`; хелпер `registerType<Interface, Impl>(c, args...)`.
- `test.cpp` — проверяет resolve фабрики, singleton-поведение,
  регистрацию готового instance, исключение при
  незарегистрированном типе, `isRegistered`, `clear` и хелпер
  `registerType`.

## Ключевые идеи

1. **`registerFactory<T>(Factory)` регистрирует фабрику.**
   Фабрика возвращает `std::shared_ptr<void>`, но `resolve<T>()`
   делает `static_pointer_cast<T>`. Это позволяет хранить
   фабрики для разных типов в одном контейнере.

2. **`registerInstance<T>(shared_ptr<T>)` регистрирует готовый
   экземпляр.**
   `resolve<T>()` вернёт его же — это singleton, созданный
   снаружи. Полезно для тестов: подменить реальную зависимость
   на мок.

3. **`resolve<T>()` кэширует результат фабрики.**
   Повторный вызов вернёт тот же объект — singleton-поведение по
   умолчанию. Если нужен transient (новый объект каждый раз) —
   уберите кэширование или добавьте флаг.

4. **`registerType<Interface, Impl>(c, args...)` — хелпер.**
   Создаёт фабрику из конструктора `Impl(args...)`. Упрощает
   регистрацию: `registerType<IFoo, FooImpl>(c, 99)` вместо
   `c.registerFactory<IFoo>([...] { return std::make_shared<FooImpl>(99); });`.

5. **`isRegistered<T>()` и `clear()` — для тестов.**
   Проверить, что тип зарегистрирован, и очистить контейнер между
   тестами. Без `clear()` состояние контейнера протекает между
   тестами.

## Ограничения

1. **Нет автоматического разрешения графа зависимостей.**
   В Boost.DI контейнер сам понимает, что `Service` зависит от
   `ILogger` и `IDatabase`, и создаёт их. Здесь зависимости
   разрешаются вручную: `auto logger = c.resolve<ILogger>(); auto
   db = c.resolve<IDatabase>(); Service svc(logger, db);`. Для
   образовательного примера это нормально — видно, что происходит.

2. **`registerType` не работает с move-only аргументами.**
   `std::make_tuple` копирует. Для move-only нужен
   `std::tuple<Args...>` с `std::forward_as_tuple` и `std::apply`
   с `std::move`.

3. **Runtime-контейнер, не compile-time.**
   В C++20 можно сделать compile-time DI (шаблонный контейнер с
   явными зависимостями), но это сложнее и менее наглядно для
   примера.

## См. также

- [DIP](../principles/solid/dip/README.md) — принцип инверсии
  зависимостей; DI Container — его практическая реализация.
- [Hollywood Principle](../principles/hollywood_principle/README.md) —
  «не звоните нам, мы позвоним вам»; DI Container — один из
  способов инверсии управления.
- [Factory Method](../../cpp20/factory_method/README.md) — фабрики,
  которые регистрируются в контейнере.
- [docs/notes.md](../../docs/notes.md) — заметки по `arch/`.