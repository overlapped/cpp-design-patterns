# C++ Design Patterns (C++11 & C++20 & C++23)

[![CI](https://github.com/overlapped/cpp-design-patterns/actions/workflows/ci.yml/badge.svg)](https://github.com/overlapped/cpp-design-patterns/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C++11](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://en.cppreference.com/w/cpp/11)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/CMake-%E2%89%A53.25-064F8C.svg)](https://cmake.org)
![GCC 14+](https://img.shields.io/badge/GCC-14%2B-blue)
![Clang 18+](https://img.shields.io/badge/Clang-18%2B-orange)

Коллекция сниппетов классических шаблонов проектирования (GoF + Publisher/Subscriber), идиом C++, принципов проектирования и архитектурных паттернов. Каждый паттерн — в отдельной папке со своей header-only реализацией, примером использования и unit-тестами на Google Test. Всё собирается CMake, есть пресеты для Debug / Release / ASan+UBSan / TSan.

Цель репозитория — учебная: показать, как один и тот же паттерн выглядит на C++11, C++20 и C++23 (с `std::variant`, `concepts`, `std::unique_ptr`, `std::function`, `std::expected`, `std::generator` и т.п.), и дать запускаемые примеры, а не «идеальный продакшн-код».

## 🚀 Быстрый старт

```bash
git clone git@github.com:overlapped/cpp-design-patterns.git
cd cpp-design-patterns

# Вариант 1: пресет (рекомендуется)
cmake --preset release
cmake --build --preset release -j"$(nproc)"
ctest --preset release --output-on-failure

# Вариант 2: вручную
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(nproc)"
ctest --test-dir build --output-on-failure
```

## 📁 Структура репозитория

```text
cpp-design-patterns/
├── CMakeLists.txt              # корневая сборка, регистрация всех паттернов
├── CMakePresets.json           # debug / release / ci-asan / ci-tsan
├── cmake/
│   ├── AddPattern.cmake        # хелпер: регистрирует example + test для паттерна
│   └── Sanitizers.cmake        # enable_sanitizers(target)
├── cpp11/                      # реализации на C++11
│   └── <pattern>/
│       ├── <pattern>.hpp
│       ├── example.cpp
│       └── test.cpp
├── cpp20/                      # те же паттерны на C++20 + jthread_stop_token, atomic_ref, policy_based_design
├── cpp23/                      # Result (std::expected), deducing this, generator, expression_templates
├── idioms/                     # RAII, Rule of Five, NVI, PImpl, Type Erasure, ...
├── arch/                       # State Machine, Memory Pool, Thread Pool, Circuit Breaker, Event Bus, ...
├── principles/                 # KISS, DRY, YAGNI, SOLID, CQS, Hollywood Principle, ...
└── docs/
    └── notes.md                # длинные пояснения, вынесенные из README
```

## 🎨 Паттерны GoF

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

## 🧩 Идиомы (`idioms/`)

| Идиома | Описание |
|---|---|
| RAII | На «сырых» `FILE*` и `std::mutex` — видно, что делает деструктор |
| Rule of Five | Все пять специальных членов + copy-and-swap |
| Rule of Zero | Ни одного специального члена — семантика выводится из полей |
| NVI | Публичный невиртуальный контракт, приватная виртуальная точка расширения |
| PImpl | Сокрытие реализации; единственная идиома, требующая `.cpp` |
| Copy-and-Swap | `operator=` через копию + `swap`, строгая гарантия исключений |
| Non-member Non-friend | Свободные функции, работающие через публичный интерфейс |
| Tag Dispatching | Выбор перегрузки по типу-тегу (`iterator_category`) |
| Erase-Remove | `v.erase(std::remove_if(...), v.end())` |
| Scope Guard | Откат действия при выходе из scope |
| SFINAE | `enable_if_t` / `void_t` для выбора перегрузки |
| Virtual Constructor | `clone()` — виртуальный «конструктор копирования» |
| Attorney-Client / Passkey | Гранулярный контроль доступа без `friend` |
| EBO / `[[no_unique_address]]` | Экономия памяти на пустых компараторах/аллокаторах |
| Hidden Friends | `friend` внутри класса для ADL и ускорения компиляции |

## 📐 Принципы проектирования (`principles/`)

Каждый — с «плохим» и «хорошим» примером, тест проверяет, что рефакторинг не сломал поведение.

- **KISS**, **DRY**, **YAGNI**
- **SOLID** — `srp/`, `ocp/`, `lsp/`, `isp/`, `dip/`
- **Composition over Inheritance**, **Law of Demeter**
- **Fail Fast**, **Separation of Concerns**, **Least Astonishment**
- **Hollywood Principle** — «не звоните нам, мы позвоним вам»
- **CQS** — Command-Query Separation
- **Tell, Don't Ask** — логика там, где данные

## 🏗️ Архитектурные паттерны (`arch/`)

- **State Machine** — `std::array<Transition, N>`, таблица фиксирована на этапе компиляции; `dispatch` возвращает `bool`.
- **Memory Pool** — минималистичный free-list без потокобезопасности; `alignas(T)` для выравнивания.
- **Double Buffering** — без потокобезопасности; в embedded атомарность обеспечивает железо.
- **Watchdog** — принимает `time_point` снаружи, детерминирован; «липкий» флаг после срабатывания.
- **Active Object** — `value_` трогает только worker-поток; клиенты получают доступ через `future`.
- **Thread Pool** — пул потоков с очередью задач и возвратом `std::future`.
- **Monitor Object** — инкапсуляция `mutex` + `condition_variable` внутри объекта.
- **Read-Write Lock** — `std::shared_mutex` против обычного `std::mutex`.
- **Double-Checked Locking** — `std::atomic` + `memory_order`; объяснение, почему без атомиков UB.
- **SPSC Ring Buffer** — lock-free очередь для одного производителя и одного потребителя.
- **Circuit Breaker** — три состояния (`Closed`/`Open`/`HalfOpen`), быстрый fail при падении сервиса.
- **Object Pool** — пул готовых объектов (например, соединений) с `acquire`/`release`.
- **DI Container** — runtime-реестр фабрик, разрешение зависимостей по типу.
- **Event Bus / Signal-Slot** — централизованная шина с RAII-хэндлом для отписки.

## ⚙️ C++20 / C++23

- **`std::jthread` + `std::stop_token`** (`cpp20/`) — кооперативная остановка, интеграция с `condition_variable_any`.
- **`std::atomic_ref`** (`cpp20/`) — атомарный доступ к неатомарной переменной.
- **Policy-Based Design** (`cpp20/`) — поведение через шаблонные политики + `concepts`.
- **Result** (`cpp23/`) — `std::expected` и монадические операции.
- **deducing this** (`cpp23/`) — эволюция CRTP без шаблонного базового класса.
- **Generator** (`cpp23/`) — `std::generator` на корутинах, `co_yield`, рекурсивные генераторы.
- **Expression Templates** (`cpp23/`) — ленивые вычисления над векторами без временных объектов.

## 🛠️ Требования

- **CMake ≥ 3.25** (для `CMakePresets.json` версии 6; для ручной сборки достаточно 3.16)
- Компилятор с поддержкой C++23: GCC ≥ 14, Clang ≥ 18 (с `libc++`), MSVC ≥ 19.29
- Python 3 — для `gtest_discover_tests`
- Google Test подтягивается автоматически через `FetchContent` (v1.14.0); если `libgtest-dev` установлен — используется системный

## 🚀 Сборка и тесты

### Через пресеты (рекомендуется)

```bash
cmake --preset debug        # build/debug, Debug + тесты + примеры
cmake --build --preset debug -j"$(nproc)"
ctest --preset debug --output-on-failure

cmake --preset release      # build/release, Release + тесты + примеры
cmake --build --preset release -j"$(nproc)"
ctest --preset release --output-on-failure
```

### Вручную

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(nproc)"
ctest --test-dir build --output-on-failure
```

### Один паттерн

```bash
cmake --build build --target example_cpp20_visitor test_cpp20_visitor
./build/example_cpp20_visitor
./build/test_cpp20_visitor
```

## 🧪 Sanitizers

```bash
# ASan + UBSan
cmake --preset ci-asan
cmake --build --preset ci-asan -j"$(nproc)"
ctest --preset ci-asan --output-on-failure

# TSan (data races)
cmake --preset ci-tsan
cmake --build --preset ci-tsan -j"$(nproc)"
ctest --preset ci-tsan --output-on-failure
```

> ASan+UBSan ловят use-after-free, утечки и UB, но **не** гонки данных — для этого TSan. Clang + `libc++` + TSan даёт ложные срабатывания, поэтому `ci-tsan` собирается на GCC.

## 📊 Статус

| Что | Значение |
|---|---|
| Паттернов GoF | 23/23 |
| Идиом | 15 |
| Принципов | 13 |
| Архитектурных паттернов | 14 |
| C++20/23 фич | 7 |
| Тестов | 250+ |
| Стандарты | C++11, C++20, C++23 |
| CI | GCC 14 (Release, ASan+UBSan, TSan), Clang 18 (ASan+UBSan) |

## 📝 Заметки и обоснования

Длинные пояснения (почему `Result` в `cpp23/`, заметки по `arch/`, почему идиомы не в `cppXX/`) вынесены в [`docs/notes.md`](docs/notes.md), чтобы README оставался лёгким.

## 🤝 Contributing

См. [CONTRIBUTING.md](CONTRIBUTING.md). Коротко: новая папка `<std>/<pattern>/` с тремя файлами (`<pattern>.hpp`, `example.cpp`, `test.cpp`), одна строка `add_pattern(<std> <pattern>)` в корневом `CMakeLists.txt`, обновление таблицы в README.

## 📄 License

MIT — см. [LICENSE](LICENSE).