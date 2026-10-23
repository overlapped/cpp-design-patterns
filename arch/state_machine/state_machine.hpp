#pragma once
#include <array>
#include <cstddef>
#include <functional>
#include <optional>
#include <utility>

// Универсальная табличная машина состояний.
// State и Event — enum class, Action — callable.
template<typename State, typename Event, std::size_t N>
class StateMachine {
public:
    using Action = std::function<void()>;

    struct Transition {
        State from;
        Event event;
        State to;
        Action action;  // может быть пустым
    };

    StateMachine(State initial, const std::array<Transition, N>& table)
        : state_(initial), table_(table) {}

    State state() const { return state_; }

    // Возвращает true, если переход найден и выполнен
    bool dispatch(Event event) {
        for (const auto& t : table_) {
            if (t.from == state_ && t.event == event) {
                if (t.action) t.action();
                state_ = t.to;
                return true;
            }
        }
        return false;   // перехода нет — состояние не меняется
    }

    // Для тестов/диагностики
    bool has_transition(State from, Event event) const {
        for (const auto& t : table_)
            if (t.from == from && t.event == event) return true;
        return false;
    }

private:
    State state_;
    std::array<Transition, N> table_;
};