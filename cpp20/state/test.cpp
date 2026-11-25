#include <gtest/gtest.h>
#include "state.hpp"

class CountingState : public State {
public:
    int* counter = nullptr;
    void handle(Context& ctx) override {
        if (counter) ++(*counter);
        ctx.setState(std::make_unique<ConcreteStateB>());
    }
};

TEST(StateTest, TransitionsBetweenStates) {
    Context ctx;
    int count = 0;
    auto s = std::make_unique<CountingState>();
    s->counter = &count;
    ctx.setState(std::move(s));
    ctx.request();
    EXPECT_EQ(count, 1);
}