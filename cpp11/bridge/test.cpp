#include <gtest/gtest.h>
#include "bridge.hpp"

class CountingImpl : public Implementor {
public:
    int calls = 0;
    void operationImpl() override { ++calls; }
};

TEST(BridgeTest, DelegatesToImpl) {
    CountingImpl impl;
    Abstraction abs(&impl);
    abs.operation();
    EXPECT_EQ(impl.calls, 1);
}