#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <cstddef>
#include <utility>
#include <algorithm>
#include <any>
#include <typeindex>

class EventBus {
public:
    using Token = std::size_t;

    // RAII-хэндл подписки: при уничтожении отписывается.
    // Хранит функцию отписки, чтобы не быть шаблонным.
    class Subscription {
    public:
        Subscription() = default;
        Subscription(std::function<void()> unsub)
            : unsub_(std::move(unsub)) {}

        Subscription(const Subscription&) = delete;
        Subscription& operator=(const Subscription&) = delete;

        Subscription(Subscription&& other) noexcept
            : unsub_(std::move(other.unsub_)) {
            other.unsub_ = nullptr;
        }

        Subscription& operator=(Subscription&& other) noexcept {
            if (this != &other) {
                unsubscribe();
                unsub_ = std::move(other.unsub_);
                other.unsub_ = nullptr;
            }
            return *this;
        }

        ~Subscription() { unsubscribe(); }

        void unsubscribe() {
            if (unsub_) {
                unsub_();
                unsub_ = nullptr;
            }
        }

        bool valid() const { return unsub_ != nullptr; }

    private:
        std::function<void()> unsub_;
    };

    // Подписка на событие типа T. Возвращает RAII-хэндл.
    template<typename T>
    [[nodiscard]] Subscription subscribe(std::function<void(const T&)> handler) {
        Token token = nextToken_++;
        handlers<T>()[token] = std::move(handler);
        return Subscription([this, token] { unsubscribe<T>(token); });
    }

    // Публикация события типа T
    template<typename T>
    void publish(const T& event) {
        auto it = handlers_.find(std::type_index(typeid(T)));
        if (it == handlers_.end()) return;

        auto& map = std::any_cast<std::unordered_map<Token, std::function<void(const T&)>>&>(it->second);
        // Копируем токены: handler может отписаться во время publish
        std::vector<Token> tokens;
        tokens.reserve(map.size());
        for (auto& [token, _] : map) tokens.push_back(token);

        for (Token token : tokens) {
            auto hIt = map.find(token);
            if (hIt != map.end()) hIt->second(event);
        }
    }

    // Отписка по токену (вызывается из Subscription)
    template<typename T>
    void unsubscribe(Token token) {
        auto it = handlers_.find(std::type_index(typeid(T)));
        if (it == handlers_.end()) return;
        auto& map = std::any_cast<std::unordered_map<Token, std::function<void(const T&)>>&>(it->second);
        map.erase(token);
    }

private:
    template<typename T>
    std::unordered_map<Token, std::function<void(const T&)>>& handlers() {
        auto key = std::type_index(typeid(T));
        auto it = handlers_.find(key);
        if (it == handlers_.end()) {
            it = handlers_.emplace(key, std::unordered_map<Token, std::function<void(const T&)>>{}).first;
        }
        return std::any_cast<std::unordered_map<Token, std::function<void(const T&)>>&>(it->second);
    }

    Token nextToken_ = 1;
    std::unordered_map<std::type_index, std::any> handlers_;
};