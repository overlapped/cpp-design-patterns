# Type Erasure

Type Erasure — идиома, позволяющая хранить объекты разных типов за
общим интерфейсом **без наследования** от общего базового класса.
Внутренний `Concept` — скрытый полиморфизм, который компилятор
генерирует через шаблон `Model<T>`. Снаружи `AnyDrawable` ведёт себя
как value-тип: копируется, перемещается, хранится в контейнерах.

## Файлы

- `type_erasure.hpp` — `AnyDrawable` с внутренними `Concept` и
  `Model<T>`, `std::unique_ptr<Concept> self_`, SFINAE-конструктор
  от любого типа с методом `draw() const`.
- `test.cpp` — проверяет диспетчеризацию к нижележащему типу,
  независимость копий, передачу владения при move и хранение
  разнородных типов в `std::vector<AnyDrawable>`.

## Ключевые идеи

1. **Внутренний `Concept` — виртуальный интерфейс.**
   `struct Concept { virtual void draw() const = 0; virtual
   std::unique_ptr<Concept> clone() const = 0; }` — скрытый от
   клиента. `AnyDrawable::draw()` вызывает `self_->draw()`.

2. **`Model<T>` — шаблонная реализация `Concept`.**
   `template<typename T> struct Model : Concept` хранит `T obj_` и
   делегирует `draw()` в `obj_.draw()`. `clone()` возвращает
   `std::make_unique<Model>(*this)` — копию конкретного типа.

3. **SFINAE-конструктор принимает любой тип с `draw() const`.**
   `template<typename T, typename = std::enable_if_t<...>>`
   `AnyDrawable(T&& obj)` отсекает сам `AnyDrawable` (защита от
   рекурсии) и требует `std::is_invocable_v<decltype(&T::draw),
   const T*>`. Тип не обязан наследоваться от `Concept`.

4. **Value-семантика через `clone()`.**
   `AnyDrawable(const AnyDrawable& other) : self_(other.self_->clone())`
   — копия независима, потому что `clone()` создаёт новый `Model<T>`
   с копией `obj_`. Move-операции — `= default`, потому что
   `unique_ptr` уже move-only. Тест `CopyIsIndependent`.

5. **Rule of Five — потому что владеем `unique_ptr<Concept>`.**
   Копирующий конструктор и копирующее присваивание реализованы
   вручную (через `clone()`), перемещающие — `= default`,
   деструктор — `= default`. Без этого компилятор сгенерировал бы
   поверхностное копирование `unique_ptr` (что невозможно) или
   удалил бы копирование.

## Ограничения

1. **`clone()` требует копируемости `T`.**
   `Model<T>::clone()` делает `std::make_unique<Model>(*this)`, что
   требует копирующего конструктора у `T`. Move-only типы (например,
   `std::unique_ptr`) в `AnyDrawable` не положатся без
   дополнительной поддержки move в `Concept`.

2. **Виртуальный вызов на каждый `draw()`.**
   Внутри `AnyDrawable::draw()` — вызов через `self_`, то есть
   динамическая диспетчеризация. Для горячих путей это может быть
   дороже, чем шаблонный полиморфизм (CRTP) или `std::variant` +
   `std::visit`.

3. **Одна операция — один `Concept`.**
   `Concept` фиксирует ровно `draw()` и `clone()`. Если нужно добавить
   вторую операцию (например, `area()`), придётся расширять `Concept`
   и `Model<T>` — или делать отдельный `AnyArea`. Это плата за
   отсутствие наследования.

## См. также

- [Rule of Five](../rule_of_five/README.md) — почему `AnyDrawable`
  нужны все пять специальных членов.
- [PImpl](../pimpl/README.md) — тоже `unique_ptr` на скрытую
  реализацию, но без шаблонного `Model<T>`.
- [CRTP](../../cpp20/crtp/README.md) — статический полиморфизм как
  альтернатива type erasure.
- [docs/notes.md](../../docs/notes.md) — заметки по идиомам.
