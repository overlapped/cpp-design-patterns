#include "state_machine.hpp"
#include <cassert>
#include <iostream>

enum class State { Idle, Running, Paused, Stopped };
enum class Event { Start, Pause, Resume, Stop };

int main() {
    int starts = 0, pauses = 0, resumes = 0;

    const std::array<StateMachine<State, Event, 4>::Transition, 4> table{{
        {State::Idle,    Event::Start,  State::Running, [&]{ ++starts;  }},
        {State::Running, Event::Pause,  State::Paused,  [&]{ ++pauses;  }},
        {State::Paused,  Event::Resume, State::Running, [&]{ ++resumes; }},
        {State::Running, Event::Stop,   State::Stopped, nullptr         },
    }};

    StateMachine<State, Event, 4> sm(State::Idle, table);

    assert(sm.state() == State::Idle);
    assert(sm.dispatch(Event::Start));
    assert(sm.state() == State::Running);
    assert(sm.dispatch(Event::Pause));
    assert(sm.state() == State::Paused);
    assert(sm.dispatch(Event::Resume));
    assert(sm.state() == State::Running);
    assert(sm.dispatch(Event::Stop));
    assert(sm.state() == State::Stopped);

    // Невалидный переход: из Stopped по Start перехода нет
    assert(!sm.dispatch(Event::Start));
    assert(sm.state() == State::Stopped);

    assert(starts == 1 && pauses == 1 && resumes == 1);
    std::cout << "State Machine example OK\n";
    return 0;
}