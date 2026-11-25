#include <gtest/gtest.h>
#include "state_machine.hpp"

enum class St { A, B, C };
enum class Ev { X, Y, Z };

using SM = StateMachine<St, Ev, 3>;

static std::array<SM::Transition, 3> makeTable(int* counter) {
    return {{
        {St::A, Ev::X, St::B, [counter]{ ++(*counter); }},
        {St::B, Ev::Y, St::C, nullptr},
        {St::C, Ev::Z, St::A, [counter]{ ++(*counter); }},
    }};
}

TEST(StateMachineTest, InitialState) {
    int c = 0;
    auto t = makeTable(&c);
    SM sm(St::A, t);
    EXPECT_EQ(sm.state(), St::A);
}

TEST(StateMachineTest, ValidTransitionChangesState) {
    int c = 0;
    auto t = makeTable(&c);
    SM sm(St::A, t);
    EXPECT_TRUE(sm.dispatch(Ev::X));
    EXPECT_EQ(sm.state(), St::B);
    EXPECT_EQ(c, 1);
}

TEST(StateMachineTest, InvalidTransitionKeepsState) {
    int c = 0;
    auto t = makeTable(&c);
    SM sm(St::A, t);
    EXPECT_FALSE(sm.dispatch(Ev::Y));   // из A по Y перехода нет
    EXPECT_EQ(sm.state(), St::A);
    EXPECT_EQ(c, 0);
}

TEST(StateMachineTest, FullCycle) {
    int c = 0;
    auto t = makeTable(&c);
    SM sm(St::A, t);
    EXPECT_TRUE(sm.dispatch(Ev::X));
    EXPECT_TRUE(sm.dispatch(Ev::Y));
    EXPECT_TRUE(sm.dispatch(Ev::Z));
    EXPECT_EQ(sm.state(), St::A);
    EXPECT_EQ(c, 2);
}

TEST(StateMachineTest, NullActionIsAllowed) {
    int c = 0;
    auto t = makeTable(&c);
    SM sm(St::B, t);
    EXPECT_TRUE(sm.dispatch(Ev::Y));
    EXPECT_EQ(sm.state(), St::C);
    EXPECT_EQ(c, 0);   // action = nullptr, счётчик не менялся
}