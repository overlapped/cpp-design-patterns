# std::jthread + std::stop_token (C++20)

`std::jthread` — это `std::thread`, который при уничтожении
автоматически вызывает `request_stop()` и `join()`. Вместе с
`std::stop_token` он даёт кооперативную остановку: поток сам
проверяет `stop_requested()` и решает, когда завершиться. Это
безопасная альтернатива `std::thread` + `std::atomic<bool>` флагу.

## Файлы

- `jthread_stop_token.hpp` — три примера: `SimpleWorker` (крутится
  до stop), `QueueWorker` (ждёт задачи, но просыпается по stop через
  `condition_variable_any::wait`), `ParallelWorkers` (несколько
  потоков, останавливаемых через общий `stop_source`).
- `test.cpp` — проверяет, что потоки работают до уничтожения,
  обрабатывают задачи, просыпаются по stop, останавливаются все
  сразу и деструктор не зависает.

## Ключевые идеи

1. **`jthread` автоматически `request_stop()` + `join()`.**
   Деструктор `std::jthread` вызывает `request_stop()` и `join()`,
   если поток joinable. Не нужно вручную писать `if (joinable())
   join()` — это главное отличие от `std::thread`.

2. **`stop_token` — кооперативный, не прерывающий.**
   `stop_requested()` возвращает `true` после `request_stop()`, но
   **не заставляет** поток остановиться. Поток сам проверяет флаг и
   решает, когда завершиться. `sleep_for` не прерывается — поток
   завершит сон и только потом проверит флаг.

3. **`condition_variable_any::wait` с `stop_token`.**
   `cv_.wait(lock, st, pred)` просыпается и по `notify`, и по
   `stop_requested()`. Это позволяет `QueueWorker` не зависнуть в
   деструкторе: `request_stop()` разбудит `wait`, поток выйдет из
   цикла, `join` завершится.

4. **`std::stop_source` для нескольких потоков.**
   `ParallelWorkers::stopAll()` вызывает `request_stop()` на каждом
   `jthread`. Все потоки завершаются, не дожидаясь деструктора
   вектора.

5. **Кооперативная остановка требует проверки во всех точках ожидания.**
   Если поток блокируется в `sleep_for` или `read`, `stop_token` не
   прервёт это ожидание. Нужно либо проверять флаг после
   возврата, либо использовать `condition_variable_any` с
   `stop_token`, либо `std::stop_callback` для пробуждения.

## Ограничения

1. **`stop_token` не прерывает блокирующие операции.**
   `sleep_for`, `read`, `recv` и т.п. не знают о `stop_token`.
   Поток завершит операцию и только потом проверит флаг. Для
   длительных операций нужен либо таймаут, либо `stop_callback`,
   который разбудит ожидание.

2. **`std::jthread` требует C++20.**
   В C++11/17 аналог — `std::thread` + `std::atomic<bool>` флаг +
   `condition_variable` с `notify_all()` при остановке. Это больше
   кода и легче ошибиться (забыть `join` → `std::terminate`).

3. **`stop_token` — one-shot.**
   После `request_stop()` отменить остановку нельзя. Если нужен
   «pause/resume», это отдельный механизм (флаг паузы +
   `condition_variable`).

## См. также

- [Active Object](../../arch/active_object/README.md) — очередь
  задач и worker-поток; `jthread` упрощает остановку.
- [Publisher/Subscriber](../../cpp20/publisher_subscriber/README.md) —
  callback'и, которые можно останавливать через `stop_token`.
- [RAII](../../idioms/raii/README.md) — `jthread` как RAII-обёртка
  над потоком.
- [docs/notes.md](../../docs/notes.md) — заметки по `cpp20/`.