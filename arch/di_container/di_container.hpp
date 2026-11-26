#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

// DI Container: контейнер зависимостей. Позволяет регистрировать
// фабрики (как создавать объект) и получать готовые экземпляры
// по типу. В отличие от compile-time подхода (шаблонный контейнер
// с явными зависимостями), runtime-контейнер хранит фабрики в
// std::unordered_map<std::type_index, ...> и умеет разрешать
// зависимости по типу в runtime.
//
// Это упрощённая версия: без автоматического разрешения графа
// зависимостей (как в Boost.DI), но с явной регистрацией фабрик.

class DIContainer {
public:
    using Factory = std::function<std::shared_ptr<void>()>;

    // Зарегистрировать фабрику для типа T
    template<typename T>
    void registerFactory(Factory factory) {
        factories_[std::type_index(typeid(T))] = std::move(factory);
    }

    // Зарегистрировать готовый экземпляр (singleton)
    template<typename T>
    void registerInstance(std::shared_ptr<T> instance) {
        instances_[std::type_index(typeid(T))] = std::move(instance);
    }

    // Получить экземпляр типа T.
    // Если есть готовый instance — вернуть его.
    // Иначе вызвать фабрику и закэшировать как singleton.
    template<typename T>
    std::shared_ptr<T> resolve() {
        auto instIt = instances_.find(std::type_index(typeid(T)));
        if (instIt != instances_.end()) {
            return std::static_pointer_cast<T>(instIt->second);
        }

        auto factIt = factories_.find(std::type_index(typeid(T)));
        if (factIt == factories_.end()) {
            throw std::runtime_error("no registration for type");
        }

        auto obj = std::static_pointer_cast<T>(factIt->second());
        instances_[std::type_index(typeid(T))] = obj;   // кэшируем
        return obj;
    }

    // Проверить, зарегистрирован ли тип
    template<typename T>
    bool isRegistered() const {
        auto key = std::type_index(typeid(T));
        return instances_.count(key) > 0 || factories_.count(key) > 0;
    }

    // Очистить все регистрации (полезно в тестах)
    void clear() {
        factories_.clear();
        instances_.clear();
    }

private:
    std::unordered_map<std::type_index, Factory> factories_;
    std::unordered_map<std::type_index, std::shared_ptr<void>> instances_;
};

// Хелпер для регистрации конкретного типа с конструктором
template<typename Interface, typename Impl, typename... Args>
void registerType(DIContainer& c, Args&&... args) {
    c.template registerFactory<Interface>(
        [args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
            return std::apply(
                [](auto&&... a) -> std::shared_ptr<void> {
                    return std::make_shared<Impl>(std::forward<decltype(a)>(a)...);
                },
                args);
        });
}