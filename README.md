# C++ Design Patterns (C++11 & C++20)

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

## 🗂️ Структура

- cpp11/<pattern>/{.hpp, example.cpp, test.cpp}
- cpp20/<pattern>/{.hpp, example.cpp, test.cpp}
- cmake/AddPattern.cmake # хелпер регистрации паттерна

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
