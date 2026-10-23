# C++ Design Patterns (C++11 & C++20)

Коллекция сниппетов классических шаблонов проектирования (GoF + Publisher/Subscriber)
на C++11 и C++20. Каждый паттерн — в отдельной папке со своей реализацией,
примером использования и unit-тестами.

## 📚 Содержание

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
| Mediator | ✅ | ✅ | Behavioral |
| Memento | ✅ | ✅ | Behavioral |
| Observer | ✅ | ✅ | Behavioral |
| State | ✅ | ✅ | Behavioral |
| Strategy | ✅ | ✅ | Behavioral |
| Template Method | ✅ | ✅ | Behavioral |
| Visitor | ✅ | ✅ | Behavioral |
| Publisher/Subscriber | ✅ | ✅ | Behavioral (variation) |
| CRTP | ✅ | ✅ | Idiom |

## 🗂️ Структура

cpp11/<pattern>/{.hpp, example.cpp, test.cpp}
cpp20/<pattern>/{.hpp, example.cpp, test.cpp}
cmake/AddPattern.cmake # хелпер регистрации паттерна


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

