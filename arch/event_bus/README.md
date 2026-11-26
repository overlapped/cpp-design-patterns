# Event Bus / Signal-Slot

Event Bus — архитектурный паттерн: централизованная шина событий.
Издатели публикуют события по типу, подписчики регистрируются на
тип. В отличие от Publisher/Subscriber и Observer, подписка
возвращает `Subscription` — RAII-хэндл, который автоматически
отписывается при уничтожении. Это решает проблему висячих
указателей: если подписчик умирает раньше издателя, его
`Subscription` уничтожается и отписывается.

## Файлы

- `event_bus.hpp` — `EventBus` с `subscribe<T>()`, `publish<T>()`,
  `unsubscribe<T>()` и вложенным `Subscription` (RAII-хэндл,
  move-only).
- `test.cpp` — проверяет подписку и публикацию, автоотписку при
  уничтожении, явную отписку, move `Subscription`, независимость
  разных типов событий и безопасную отписку во время `publish`.

## Ключевые идеи

1. **`Subscription` — RAII-хэндл.**
   При уничтожении вызывает `unsubscribe()`. Это решает проблему
   висячих указателей: если подписчик умирает раньше издателя,
   его `Subscription` уничтожается и отписывается. Тест
   `SubscriptionAutoUnsubscribesOnDestroy` проверяет это.

2. **`Subscription` move-only.**
   Копирование запрещено, перемещение разрешено. При перемещении
   старый `Subscription` становится невалидным (`unsub_ == nullptr`).
   Тест `MoveSubscription`.

3. **`publish` копирует токены перед вызовом handler'ов.**
   Это позволяет handler'у отписаться во время `publish` — тест
   `UnsubscribeDuringPublishIsSafe` проверяет это. Без копирования
   итерация по `unordered_map` сломалась бы при `erase`.

4. **Разные типы событий независимы.**
   `EventA` и `EventB` хранятся в разных `unordered_map`, ключ —
   `std::type_index`. Тест `DifferentEventTypesAreIndependent`
   проверяет, что публикация `EventA` не вызывает подписчиков
   `EventB`.

5. **`std::any` для хранения map разных типов.**
   `handlers_` хранит `std::any`, а `handlers<T>()` делает
   `any_cast` к нужному типу. Альтернатива — шаблонный
   `EventBus<T>`, но тогда нельзя подписаться на разные типы в
   одном bus.

## Ограничения

1. **`std::any_cast` в runtime.**
   Если тип не совпадает, будет исключение. В этой версии
   `handlers<T>()` создаёт map при первом обращении, так что
   `any_cast` всегда корректен.

2. **Нет потокобезопасности.**
   `subscribe`/`publish`/`unsubscribe` не защищены мьютексом. Для
   многопоточного сценария нужен `std::mutex` вокруг `handlers_`
   и `nextToken_`.

3. **`publish` копирует токены.**
   Это O(N) на каждую публикацию. Для горячих путей можно
   использовать `std::vector<Token>` с резервом или lock-free
   структуру.

## См. также

- [Publisher/Subscriber](../../cpp20/publisher_subscriber/README.md) —
  простая версия без RAII-хэндла.
- [Observer](../../cpp20/observer/README.md) — GoF-версия с
  `attach`/`detach`.
- [RAII](../../idioms/raii/README.md) — `Subscription` использует
  ту же идиому.
- [docs/notes.md](../../docs/notes.md) — заметки по `arch/`.