# Policy-Based Design

Policy-Based Design — подход, при котором поведение класса
parameterizуется шаблонными политиками, а не наследованием. Каждая
политика — отдельный аспект (логирование, хранение, проверка), и их
можно комбинировать свободно. В C++20 политики проверяются через
`concepts`: если политика не удовлетворяет контракту, компилятор
скажет об этом явно, а не в глубине шаблонных ошибок.

## Файлы

- `policy_based_design.hpp` — политики `ConsoleLogger`/`SilentLogger`
  (логирование), `VectorStorage`/`ArrayStorage` (хранение),
  `NoCheck`/`PositiveCheck` (проверка); `concepts` `LoggerPolicy` и
  `StoragePolicy`; класс `Repository<T, Logger, Storage, Check>`.
- `test.cpp` — проверяет политики по умолчанию, `SilentLogger` без
  вывода, ёмкость `ArrayStorage`, `PositiveCheck` отвергает
  отрицательные, комбинацию политик и `static_assert` на concepts.

## Ключевые идеи

1. **Политики — отдельные аспекты.**
   `Logger` (логирование), `Storage` (хранение), `Check` (проверка).
   Каждая политика — независимый шаблонный параметр, и их можно
   комбинировать свободно: `Repository<int, SilentLogger,
   ArrayStorage, PositiveCheck<int>>`.

2. **`LoggerPolicy` concept проверяет контракт.**
   `template<typename T> concept LoggerPolicy = requires(const T& t,
   const std::string& s) { { t.log(s) } -> std::same_as<void>; };`
   Если передать `int` вместо логгера, компилятор скажет
   «`int` does not satisfy `LoggerPolicy`», а не «no member named
   `log` in `int`» в глубине шаблона.

3. **`StoragePolicy` concept проверяет `add`/`size`/`get`.**
   `VectorStorage` и `ArrayStorage` оба удовлетворяют: у первого
   `std::vector`, у второго — фиксированный массив на 16 элементов.

4. **`Check` — шаблонный параметр, не concept.**
   `NoCheck<T>` всегда возвращает `true`, `PositiveCheck<T>` —
   `v > T{}`. Для примера достаточно `typename Check = NoCheck<T>`;
   в реальном коде можно сделать concept `CheckPolicy`.

5. **`ArrayStorage` имеет фиксированную ёмкость.**
   Тест `ArrayStorageCapacity` проверяет, что после 20 добавлений
   размер 16 — лишние отбрасываются. Это демонстрирует, что
   политика хранения может менять не только тип контейнера, но и
   поведение при переполнении.

## Ограничения

1. **Больше шаблонных параметров — больше сложность.**
   `Repository<int, SilentLogger, ArrayStorage, PositiveCheck<int>>`
   читается тяжело. В C++20 это частично лечится `concepts`, но не
   полностью. В реальном коде часто используют `using`-алиасы для
   типовых комбинаций.

2. **Политики не должны иметь состояние.**
   В примере `Logger`, `Storage`, `Check` — пустые или простые
   структуры. Если политика хранит состояние, `Repository`
   становится тяжелее, и комбинации усложняются.

3. **Не заменяет наследование для динамического полиморфизма.**
   Политики выбираются на этапе компиляции. Если нужно менять
   поведение в runtime — это Strategy, а не Policy-Based Design.

## См. также

- [CRTP](../../cpp20/crtp/README.md) — статический полиморфизм;
  Policy-Based Design часто использует CRTP для `mixin`-политик.
- [Expression Templates](../expression_templates/README.md) —
  тоже шаблонные параметры, но для построения дерева выражений, а
  не для выбора поведения.
- [Strategy](../../cpp20/strategy/README.md) — динамическая
  альтернатива: поведение меняется в runtime.
- [docs/notes.md](../../docs/notes.md) — заметки по `cpp20/`.