#include <gtest/gtest.h>
#include "state.hpp"

class CountingState : public State {
public:
    int* counter = nullptr;
    void handle(Context& ctx) override {
        if (counter) ++(*counter);
        ctx.setState(std::unique_ptr<State>(new ConcreteStateB()));
    }
};

TEST(StateTest, TransitionsBetweenStates) {
    Context ctx;
    int count = 0;
    auto s = std::unique_ptr<CountingState>(new CountingState());
    s->counter = &count;
    ctx.setState(std::move(s));
    ctx.request();
    EXPECT_EQ(count, 1);
}