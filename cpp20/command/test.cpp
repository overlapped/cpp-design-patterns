#include <gtest/gtest.h>
#include "command.hpp"

TEST(CommandTest, ExecutesAndUndoes) {
    Light light;
    RemoteControl remote;
    remote.submit(std::make_unique<LightOnCommand>(light));
    EXPECT_EQ(light.state, "on");
    EXPECT_TRUE(remote.undoLast());
    EXPECT_EQ(light.state, "off");
}