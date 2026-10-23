# Contributing

Спасибо, что хотите помочь! Правила простые.

## Как добавить новый паттерн

1. Создайте папки `cpp11/<pattern>/` и `cpp20/<pattern>/`.
2. В каждой папке — три файла:
   - `<pattern>.hpp` — реализация (header-only)
   - `example.cpp` — минимальный `main()` с `assert`
   - `test.cpp` — Google Test-тесты
3. Добавьте регистрацию в корневой `CMakeLists.txt`:
   ```cmake
   add_pattern(cpp11 <pattern>)
   add_pattern(cpp20 <pattern>)
4. Обновите таблицу в README.md.
5. Убедитесь, что локально проходит:

```bash
cmake -S . -B build
cmake --build build -j
ctest --test-dir build --output-on-failure
```
