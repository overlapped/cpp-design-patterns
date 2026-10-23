# C++ Design Patterns (C++11 & C++20)

[![CI](https://github.com/overlapped/cpp-design-patterns/actions/workflows/ci.yml/badge.svg)](https://github.com/overlapped/cpp-design-patterns/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C++11](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://en.cppreference.com/w/cpp/11)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-%E2%89%A53.25-064F8C.svg)](https://cmake.org)

Коллекция сниппетов классических шаблонов проектирования (GoF + Publisher/Subscriber)
на C++11 и C++20. Каждый паттерн — в отдельной папке со своей реализацией,
примером использования и unit-тестами.

## 📚 Содержание

### Паттерны

| Паттерн | C++11 | C++20 | Категория |
|---|---|---|---|
| Singleton | ✅ | ✅ | Creational |
| Factory Method | ✅ | ✅ | Creational |
| Abstract Factory | ✅ | ✅ | Creational |
| Builder | ✅ | ✅ | Creational |
| Prototype | ✅ | ✅ | Creational |
| Adapter | ✅ | ✅ | Structural |
| Bridge | ✅ | ✅ | Structural |
| Composite | ✅ | ✅ | Structural |
| Decorator | ✅ | ✅ | Structural |
| Facade | ✅ | ✅ | Structural |
| Flyweight | ✅ | ✅ | Structural |
| Proxy | ✅ | ✅ | Structural |
| Chain of Responsibility | ✅ | ✅ | Behavioral |
| Command | ✅ | ✅ | Behavioral |
| Iterator | ✅ | ✅ | Behavioral |
| Interpreter | ✅ | ✅ | Behavioral |
| Mediator | ✅ | ✅ | Behavioral |
| Memento | ✅ | ✅ | Behavioral |
| Observer | ✅ | ✅ | Behavioral |
| State | ✅ | ✅ | Behavioral |
| Strategy | ✅ | ✅ | Behavioral |
| Template Method | ✅ | ✅ | Behavioral |
| Visitor | ✅ | ✅ | Behavioral |
| Publisher/Subscriber | ✅ | ✅ | Behavioral (variation) |
| CRTP | ✅ | ✅ | Idiom |

### Идиомы

- RAII специально показан на «сырых» FILE* и std::mutex, чтобы было видно, что именно делает деструктор. В реальном коде используйте std::unique_ptr, std::lock_guard, std::fstream — они реализуют ту же идиому.
- Rule of Five демонстрирует все пять специальных функций и copy-and-swap для operator=. Это даёт строгую гарантию исключений: если new бросит внутри копирующего конструктора tmp, исходный объект остаётся нетронутым.
- Обе идиомы не зависят от стандарта (C++11 уже всё это поддерживает), поэтому лежат в idioms/, а не в cpp11//cpp20/. В C++20-версии можно было бы добавить std::span для доступа к данным или std::byte, но это уже вкусовщина, а не суть идиомы.
- PImpl — единственная идиома, где нужен отдельный .cpp; остальные header-only. В таблице идиом её стоит пометить как «требует compilation unit».
- NVI — это по сути Template Method, но на уровне идиомы: публичный невиртуальный метод задаёт контракт, приватный виртуальный — точку расширения. Хорошо смотрится рядом с RAII и Rule of Five как «база C++».

## Замечания

- Result в C++23 становится идиоматичным благодаря std::expected и его монадическим операциям (and_then, transform, or_else). Это не «паттерн GoF», а современный подход к обработке ошибок, который часто ставят в один ряд с паттернами.
- deducing this — это эволюция CRTP, а не отдельный паттерн. Но в образовательном репозитории логично показать его рядом с cpp20/crtp/, чтобы читатель увидел разницу: шаблонный базовый класс vs нешаблонный с this auto&&.
- State Machine использует std::array<Transition, N> — таблица переходов фиксирована на этапе компиляции, что типично для embedded. Если хочется динамики — замените на std::vector и добавьте add_transition. dispatch возвращает bool, чтобы вызывающий код знал, был ли переход; это лучше, чем молча игнорировать невалидное событие.
- Memory Pool — минималистичный free-list без потокобезопасности. В реальном embedded этого обычно достаточно (один поток/прерывание), но если понадобится — добавьте std::mutex или используйте lock-free free-list. reinterpret_cast<T*>(node->storage) корректен, потому что storage выровнен по alignof(T) через alignas(T).
- Оба паттерна не зависят от стандарта (C++11 достаточно), поэтому лежат в arch/, а не в cppXX/. Это архитектурный слой, а не языковой.
- Тест RespectsAlignment может показаться избыточным, но он ловит класс ошибок, который ASan не всегда видит (misaligned access) — полезно для embedded-контекста.
- Double Buffering — без потокобезопасности, как и MemoryPool. Для многопоточного сценария либо используйте std::atomic-индексы, либо оберните в мьютекс; но тогда это уже другой паттерн (Active Object). В embedded double buffering часто делается через DMA и прерывание — там атомарность обеспечивает железо.
- Watchdog специально принимает time_point снаружи. Это делает его детерминированным и тестируемым без sleep. В реальном коде поверх него пишется тонкая обёртка, которая берёт steady_clock::now() и вызывает kick/expired. Тест ExpiredIsSticky проверяет важное свойство: после срабатывания флаг не сбрасывается сам — сброс только через start() или kick().
- Ключевое свойство Active Object: value_ трогает только worker_-поток. Клиенты никогда не получают прямой доступ — только через future. Это делает класс потокобезопасным без единого atomic в публичном API.
- Деструктор должен дождаться рабочего потока (join) и опустошить очередь (или отменить задачи). В примере мы не отменяем задачи, а даём им выполниться — для коротких задач это проще. В реальном коде можно добавить флаг cancelled_ и проверять его в run().
- std::packaged_task + std::function — стандартный способ «упаковать» задачу так, чтобы она могла вернуть значение через future. Обратите внимание: task захватывается в лямбду как shared_ptr, иначе она была бы уничтожена до выполнения.
- Тест ConcurrentIncrementsFromManyThreads специально вызывает .get() после каждого increment(), чтобы гарантировать линейный порядок и проверить именно сериализацию, а не конкурентность. Если хочется проверить «настоящую» параллельность — уберите .get() и в конце дождитесь всех futures, но тогда порядок будет недетерминирован, и проверять надо только итоговую сумму.
- ASan+UBSan здесь особенно полезны: гонки данных они не ловят (для этого нужен TSan), но use-after-free и утечки в std::thread/std::promise — вполне. Если хотите ловить гонки, добавьте отдельный пресет ci-tsan с -fsanitize=thread — но это уже следующая итерация.

## 🛠️ Требования

- CMake ≥ 3.16
- Компилятор с поддержкой C++20 (GCC ≥ 10, Clang ≥ 12, MSVC ≥ 19.29)
- Python 3 (для `gtest_discover_tests`)

Google Test подтягивается автоматически через `FetchContent`.

## 🚀 Сборка и тесты

```bash
cmake -S . -B build
cmake --build build -j
ctest --test-dir build --output-on-failure

cmake --build build --target test_cpp20_visitor
./build/test_cpp20_visitor
./build/example_cpp20_visitor
```

## 🧪 Sanitizers

Локально:

```bash
cmake --preset ci-asan
cmake --build --preset ci-asan -j
ctest --preset ci-asan

cmake -S . -B build/asan -DSANITIZE=address,undefined
cmake --build build/asan -j
ctest --test-dir build/asan --output-on-failure
```

### 🚀 Проверка

```bash
cmake --preset debug
cmake --build --preset debug -j
ctest --test-dir build/debug --output-on-failure

cmake --preset ci-asan
cmake --build --preset ci-asan -j
ctest --preset ci-asan
```

## 📊 Статус

| Что | Значение |
|---|---|
| Паттернов GoF | 23/23 |
| Идиом | 13 |
| Тестов | 110+ |
| Стандарты | C++11, C++20, C++23 |
| CI | GCC 14, Clang 18, ASan+UBSan |
